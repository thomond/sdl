from PIL import Image
import numpy as np
import csv

def doit(sprite_sheet_path):
    # Reload the sprite sheet after code execution state reset
    #sprite_sheet_path = "/mnt/data/image.png"
    sprite_sheet = Image.open(sprite_sheet_path).convert("RGBA")

    # Constants based on sprite sheet layout (6 columns x 4 rows)
    frame_width = 64
    frame_height = 64
    columns = 6
    rows = 4

    # Prepare list for bounding boxes
    bounding_boxes = []

    # Extract bounding box for each frame
    for row in range(rows):
        for col in range(columns):
            x0 = col * frame_width
            y0 = row * frame_height
            frame = sprite_sheet.crop((x0, y0, x0 + frame_width, y0 + frame_height))
            np_frame = np.array(frame)

            # Get alpha channel
            alpha = np_frame[:, :, 3]
            non_transparent = np.argwhere(alpha > 0)

            if non_transparent.size > 0:
                y_min, x_min = non_transparent.min(axis=0)
                y_max, x_max = non_transparent.max(axis=0)
                bbox = (x_min + x0, y_min + y0, x_max - x_min + 1, y_max - y_min + 1)
            else:
                bbox = (x0, y0, 0, 0)  # Fully transparent

            frame_index = row * columns + col
            bounding_boxes.append((frame_index, *bbox))

    # Convert absolute coordinates to relative (within each frame)
    relative_boxes = []

    for frame_index, abs_x, abs_y, w, h in bounding_boxes:
        frame_col = frame_index % columns
        frame_row = frame_index // columns
        frame_x0 = frame_col * frame_width
        frame_y0 = frame_row * frame_height

        rel_x = abs_x - frame_x0
        rel_y = abs_y - frame_y0

        relative_boxes.append((frame_index, rel_x, rel_y, w, h))

    # Save updated bounding boxes to a new CSV file
    relative_csv_path = "collision_rects_relative.csv"
    with open(relative_csv_path, "w", newline="") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(["frame", "x", "y", "w", "h"])
        writer.writerows(relative_boxes)

    relative_csv_path
