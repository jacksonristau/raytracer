import json
import subprocess
import shutil
import sys
import os
import math

def render_animation(scene_path, num_frames, output_dir="frames"):
    # Load scene
    with open(scene_path, 'r') as f:
        scene = json.load(f)

    # Store original rotation
    mesh = scene["meshes"][0]
    original_rotation = mesh["transform"]["rotate"].copy()
    original_translation = mesh["transform"]["translate"].copy()
    # Delete output directory if it exists
    if os.path.exists(output_dir):
        shutil.rmtree(output_dir)
    # Create output directory
    os.makedirs(output_dir, exist_ok=True)

    # Get project root (parent of scripts/)
    script_dir = os.path.dirname(os.path.abspath(__file__))
    project_root = os.path.dirname(script_dir)

    # Convert scene_path to absolute
    scene_path = os.path.abspath(scene_path)

    for i in range(num_frames):
        # Linear interpolation: frame 0 = 0°, frame N-1 = 360° - step
        # This way a loop is seamless (frame N would equal frame 0)
        t = i / num_frames
        y_rotation = t * 360.0
        y_translation = math.sin(t * 2 * math.pi)

        # Update rotation (preserve X and Z, interpolate Y)
        mesh["transform"]["rotate"] = [
            original_rotation[0],
            original_rotation[1] + y_rotation,
            original_rotation[2]
        ]

        mesh["transform"]["translate"] = [
            original_translation[0],
            original_translation[1] + y_translation,
            original_translation[2]
        ]

        # Write modified scene
        with open(scene_path, 'w') as f:
            json.dump(scene, f, indent=2)

        print(f"Rendering frame {i + 1}/{num_frames} (rotation Y = {y_rotation:.1f}°)")
        print(f"(translation Y = {y_translation:.1f}°)")


        # Call raytracer-cli from build dir (where relative paths in scene.json expect)
        build_dir = os.path.join(project_root, "build/x64-Release")
        raytracer = os.path.join(build_dir, "apps/raytracer-cli/raytracer-cli")
        print(raytracer)
        result = subprocess.run(
            [raytracer, scene_path],
            cwd=build_dir,
            capture_output=True,
            text=True
        )

        if result.returncode != 0:
            print(f"Error rendering frame {i}:")
            print(result.stderr)
            continue

        # Rename render.ppm to frames/render_001.ppm etc.
        src = os.path.join(build_dir, "render.ppm")
        dst = os.path.join(output_dir, f"frame_{i:03d}.ppm")

        if os.path.exists(src):
            shutil.move(src, dst)
            print(f"  Saved: {dst}")
        else:
            print(f"  Warning: render.ppm not found")

    # Restore original scene
    mesh["transform"]["rotate"] = original_rotation
    mesh["transform"]["translate"] = original_translation
    with open(scene_path, 'w') as f:
        json.dump(scene, f, indent=2)

    result = subprocess.run(
            ["ffmpeg", "-framerate", "30", "-i", "frame_%03d.ppm", "-vf", "split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse", "output.gif"],
            cwd=output_dir,
            capture_output=True,
            text=True
        )

    if result.returncode != 0:
        print(f"Error rendering frame {i}:")
        print(result.stderr)

    print(f"\nDone! {num_frames} frames saved to {output_dir}/")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python render_animation.py <scene.json> <num_frames> [output_dir]")
        print("Example: python render_animation.py ../../files/scene.json 36")
        sys.exit(1)

    scene_path = sys.argv[1]
    num_frames = int(sys.argv[2])
    output_dir = sys.argv[3] if len(sys.argv) > 3 else "frames"

    render_animation(scene_path, num_frames, output_dir)
