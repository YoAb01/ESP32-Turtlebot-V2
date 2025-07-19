#!/usr/bin/env python3

import sys
import pygame
import socket

# ====== CONFIG ======
ESP32_IP = "192.168.1.10"
ESP32_PORT = 4201

# ====== INIT SOCKET ======
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# ====== INIT PYGAME & JOYSTICK ======
pygame.init()
pygame.joystick.init()

if pygame.joystick.get_count() == 0:
    print("No joystick connected")
    sys.exit()

joystick = pygame.joystick.Joystick(0)
joystick.init()

# ====== VISUALIZER WINDOW ======
WIDTH, HEIGHT = 600, 400
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Joystick Visualizer")
font = pygame.font.SysFont(None, 24)
clock = pygame.time.Clock()


def draw_stick_circle(center_x, center_y, axis_x_val, axis_y_val, radius, label):
    # Title
    title = font.render("JoyStick Simple Visualizer", True, (255, 0, 0))
    screen.blit(title, (WIDTH // 3, 15))
    # Outer circle
    pygame.draw.circle(screen, (100, 100, 255), (center_x, center_y), radius, 2)
    pygame.draw.line(screen, (80, 80, 80), (center_x - radius, center_y), (center_x + radius, center_y), 1)
    pygame.draw.line(screen, (80, 80, 80), (center_x, center_y - radius), (center_x, center_y + radius), 1)
    # Dot (stick position)
    dot_x = int(center_x + axis_x_val * radius)
    dot_y = int(center_y + axis_y_val * radius)
    pygame.draw.circle(screen, (0, 200, 255), (dot_x, dot_y), 8)
    # Label
    text = font.render(label, True, (255, 255, 255))
    screen.blit(text, (center_x - text.get_width() // 2, center_y + radius + 10))


def scale_to_int8(val):
    return max(-127, min(127, int(val * 127)))


# ====== MAIN LOOP ======
while True:
    screen.fill((20, 20, 20))
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

    # Get axes (defaulting missing axes to 0)
    ax0 = scale_to_int8(-joystick.get_axis(0))
    ax1 = scale_to_int8(-joystick.get_axis(1))
    ax2 = scale_to_int8(joystick.get_axis(2)) if joystick.get_numaxes() > 2 else 0
    ax3 = scale_to_int8(joystick.get_axis(3)) if joystick.get_numaxes() > 3 else 0

    # Get buttons (first 4)
    btn1 = joystick.get_button(0)
    btn2 = joystick.get_button(1)
    btn3 = joystick.get_button(2)
    btn4 = joystick.get_button(3)

    # Build expected message format
    message = f"AXIS:{ax0}:{ax1}:{ax2}:{ax3};BTN:{btn1}:{btn2}:{btn3}:{btn4}"
    print(f"Sending {message}")

    sock.sendto(message.encode(), (ESP32_IP, ESP32_PORT))

    # Draw left and right sticks
    draw_stick_circle(180, 180, joystick.get_axis(0), joystick.get_axis(1), 60, "Left Stick")
    draw_stick_circle(420, 180, joystick.get_axis(3), joystick.get_axis(2), 60, "Right Stick")

    pygame.display.flip()
    clock.tick(60)
