#!/bin/bash

# This is a simple program to easly select songs from a playlist to play based on:
# fzf, mpv and yt-dlp
#
BASE_DIR="$HOME/Documents/MainFolder/github/small_Projects/music_player/playlists"

if [[ ! -d "$BASE_DIR" ]]; then
    echo "Error: Directory $BASE_DIR not found."
    exit 1
fi

category_file=$(ls "$BASE_DIR" | fzf --prompt="Select Playlist: " --height=15% --reverse)

if [[ -z "category_file" ]]; then
    echo "Error: Music list not found at $BASE_DIR"
    exit 1
fi

FULL_PATH="$BASE_DIR/$category_file"

# We print the array, pipe it to fzf, and save the choice to a variable
choice=$(cat "$FULL_PATH" | fzf --prompt="Select Station: " --height=10% --reverse)

# Extract the URL (everything after the '|') and play it
# If the user pressed ESC, 'choice' will be empty, so we check first

if [ -z "$choice" ]; then
    echo "No selection. Exiting."
    exit 2
fi

# Use 'cut' to get everything after the '|' character
# Then use 'xargs' to trim any accidental spaces
url=$(echo "$choice" | cut -d'|' -f2 | xargs)

echo "Now Playing: $url"

mpv --no-video "$url"
