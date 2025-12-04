import numpy as np
from PIL import Image, ImageDraw
from math import pi, cos, sin

def pentagram(img, x, y, r, thickness, color):
    draw = ImageDraw.Draw(img)
    color = tuple(color)
    
    draw.ellipse([x - r, y - r, x + r, y + r], outline=color, width=thickness)
    
    cord_versh = []
    for i in range(5):
        phi = (pi/5)*(2*i+3/2)
        node_i = [int(x+r*cos(phi)), int(y+r*sin(phi))]
        cord_versh.append(node_i)
    
    for i in range(5):
        start = cord_versh[i]
        end = cord_versh[(i+2) % 5]
        draw.line([start[0], start[1], end[0], end[1]], fill=color, width=thickness)
    
    return img


def swap(img, x0, y0, x1, y1, width):
    img_copy = img.copy()
    
    cr_1 = img_copy.crop((x0, y0, x0+width, y0+width))
    cr_2 = img_copy.crop((x1, y1, x1+width, y1+width))
    
    cr_1 = cr_1.rotate(-90)
    cr_2 = cr_2.rotate(-90)
    
    img_copy.paste(cr_2, (x0, y0))
    img_copy.paste(cr_1, (x1, y1))
    
    img_copy = img_copy.rotate(-90)
    
    return img_copy


def avg_color(img, x0, y0, x1, y1):
    img_copy = img.copy()
    img_copy = img_copy.convert("RGB")
    
    result = img_copy.copy()
    pixels = result.load()
    original_pixels = img_copy.load()
    
    for y in range(y0, y1 + 1):
        for x in range(x0, x1 + 1):
            neighbors = []
            
            for dy in [-1, 0, 1]:
                for dx in [-1, 0, 1]:
                    if dx == 0 and dy == 0:
                        continue
                    
                    nx, ny = x + dx, y + dy
                    
                    if 0 <= nx < img_copy.width and 0 <= ny < img_copy.height:
                        neighbors.append(original_pixels[nx, ny])
            
            if neighbors:
                avg_r = int(sum(n[0] for n in neighbors) / len(neighbors))
                avg_g = int(sum(n[1] for n in neighbors) / len(neighbors))
                avg_b = int(sum(n[2] for n in neighbors) / len(neighbors))
                
                pixels[x, y] = (avg_r, avg_g, avg_b)
    
    return result


def col_pix(img, x0, x1, y0, y1):
    pix_col = []
    
    y = y0 - 1
    if 0 <= y < img.height:
        for x in range(x0, x1 + 1):
            if 0 <= x < img.width:
                pix_col.append(img.getpixel((x, y)))
    
    y = y1 + 1
    if 0 <= y < img.height:
        for x in range(x0, x1 + 1):
            if 0 <= x < img.width:
                pix_col.append(img.getpixel((x, y)))
    
    x = x0 - 1
    if 0 <= x < img.width:
        for y in range(y0, y1 + 1):
            if 0 <= y < img.height:
                pix_col.append(img.getpixel((x, y)))
    
    x = x1 + 1
    if 0 <= x < img.width:
        for y in range(y0, y1 + 1):
            if 0 <= y < img.height:
                pix_col.append(img.getpixel((x, y)))
    
    return pix_col
