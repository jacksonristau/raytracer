#!/usr/bin/env python3
"""Render a turntable animation of a scene and save it as a GIF.

Spins one mesh through a full rotation, renders each frame with raytracer-cli,
and stitches the frames together with ffmpeg. The scene file is never modified.

    python scripts/render_animation.py files/scenes/glass_bunny.json
    python scripts/render_animation.py files/scenes/glass_bunny.json --size 400 400 --frames 24 -o bunny.gif
"""
import argparse
import copy
import json
import shutil
import subprocess
import sys
import tempfile
import time
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent
AXES = {"x": 0, "y": 1, "z": 2}


def fail(message):
    sys.exit(f"error: {message}")


def find_raytracer(explicit):
    if explicit:
        path = Path(explicit).resolve()
        if not path.is_file():
            fail(f"raytracer not found: {path}")
        return path

    # any build directory works (build/, build/x64-Release/, out/build/...), newest wins
    candidates = [
        p
        for root in (PROJECT_ROOT / "build", PROJECT_ROOT / "out")
        if root.is_dir()
        for name in ("raytracer-cli", "raytracer-cli.exe")
        for p in root.rglob(name)
        if p.is_file()
    ]
    if not candidates:
        fail("couldn't find raytracer-cli. Build the project first (see README) or pass --raytracer PATH")
    return max(candidates, key=lambda p: p.stat().st_mtime)


def parse_args():
    parser = argparse.ArgumentParser(
        description="Render a turntable animation of a scene and save it as a GIF.")
    parser.add_argument("scene", type=Path, help="scene JSON file")
    parser.add_argument("-o", "--output", type=Path,
                        help="output GIF (default: <scene name>.gif in the current directory)")
    parser.add_argument("-n", "--frames", type=int, default=36, help="number of frames (default: 36)")
    parser.add_argument("--fps", type=int, default=10, help="GIF frame rate (default: 10)")
    parser.add_argument("--degrees", type=float, default=360.0,
                        help="total rotation across the animation (default: 360, a seamless loop)")
    parser.add_argument("--axis", choices=AXES, default="y", help="rotation axis (default: y)")
    parser.add_argument("--mesh", type=int, default=0,
                        help="index of the mesh in the scene's \"meshes\" list to rotate (default: 0)")
    parser.add_argument("--size", type=int, nargs=2, metavar=("W", "H"),
                        help="override the camera resolution, handy for quick previews")
    parser.add_argument("-S", "--samples", type=int,
                        help="anti-aliasing samples per axis, passed to raytracer-cli")
    parser.add_argument("--keep-frames", type=Path, metavar="DIR",
                        help="also save the individual PPM frames to DIR")
    parser.add_argument("--raytracer", help="path to raytracer-cli (default: newest one under build/)")
    args = parser.parse_args()
    if args.frames < 1:
        parser.error("--frames must be at least 1")
    if args.fps < 1:
        parser.error("--fps must be at least 1")
    return args


def load_scene(path, mesh_index):
    try:
        scene = json.loads(path.read_text())
    except FileNotFoundError:
        fail(f"scene not found: {path}")
    except json.JSONDecodeError as e:
        fail(f"{path} is not valid JSON: {e}")

    meshes = scene.get("meshes", [])
    if not meshes:
        fail(f"{path} has no meshes to rotate")
    if not 0 <= mesh_index < len(meshes):
        fail(f"--mesh {mesh_index} is out of range, the scene has {len(meshes)} mesh(es)")

    # raytracer-cli needs all three keys whenever a transform is present
    transform = meshes[mesh_index].setdefault("transform", {})
    transform.setdefault("translate", [0, 0, 0])
    transform.setdefault("rotate", [0, 0, 0])
    transform.setdefault("scale", [1, 1, 1])
    return scene


def render_frames(args, scene, raytracer, frames_dir):
    # relative asset paths in scenes are resolved from the raytracer's directory (see README)
    workdir = raytracer.parent
    render = workdir / "render.ppm"
    frame_scene_path = frames_dir / "scene.json"
    base_rotation = scene["meshes"][args.mesh]["transform"]["rotate"]
    axis = AXES[args.axis]

    command = [str(raytracer), str(frame_scene_path)]
    if args.samples is not None:
        command += ["-S", str(args.samples)]

    start = time.monotonic()
    for i in range(args.frames):
        angle = args.degrees * i / args.frames
        frame = copy.deepcopy(scene)
        rotation = list(base_rotation)
        rotation[axis] += angle
        frame["meshes"][args.mesh]["transform"]["rotate"] = rotation
        if args.size:
            frame["camera"]["imsize"] = args.size
        frame_scene_path.write_text(json.dumps(frame))

        frame_start = time.monotonic()
        render.unlink(missing_ok=True)
        result = subprocess.run(command, cwd=workdir, capture_output=True, text=True)
        if result.returncode != 0 or not render.exists():
            output = (result.stdout + result.stderr).strip()
            fail(f"raytracer-cli failed on frame {i + 1} (exit code {result.returncode}):\n{output}")
        shutil.move(render, frames_dir / f"frame_{i:04d}.ppm")

        elapsed = time.monotonic() - start
        remaining = elapsed / (i + 1) * (args.frames - i - 1)
        print(f"frame {i + 1}/{args.frames}  {args.axis} = {angle:6.1f}°  "
              f"{time.monotonic() - frame_start:5.1f}s  (~{remaining:.0f}s left)", flush=True)


def make_gif(frames_dir, fps, output):
    output.parent.mkdir(parents=True, exist_ok=True)
    result = subprocess.run(
        ["ffmpeg", "-y", "-loglevel", "error", "-framerate", str(fps),
         "-i", str(frames_dir / "frame_%04d.ppm"),
         "-vf", "split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse",
         "-loop", "0", str(output)],
        capture_output=True, text=True)
    if result.returncode != 0:
        fail(f"ffmpeg failed:\n{result.stderr.strip()}")


def main():
    args = parse_args()
    if shutil.which("ffmpeg") is None:
        fail("ffmpeg is not on your PATH (macOS: brew install ffmpeg, Ubuntu: apt install ffmpeg)")

    scene = load_scene(args.scene, args.mesh)
    raytracer = find_raytracer(args.raytracer)
    output = (args.output or Path(args.scene.stem + ".gif")).resolve()
    print(f"using {raytracer}")

    with tempfile.TemporaryDirectory(prefix="raytracer-frames-") as tmp:
        frames_dir = Path(tmp)
        render_frames(args, scene, raytracer, frames_dir)
        make_gif(frames_dir, args.fps, output)
        if args.keep_frames:
            args.keep_frames.mkdir(parents=True, exist_ok=True)
            for frame in frames_dir.glob("frame_*.ppm"):
                shutil.copy2(frame, args.keep_frames / frame.name)
            print(f"frames saved to {args.keep_frames.resolve()}")

    print(f"saved {output}")


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        sys.exit("\ninterrupted")
