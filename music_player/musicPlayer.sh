#!/bin/bash

# This is a simple program to easly select songs from a playlist to play based on:
# fzf, mpv and yt-dlp

# features to add:
# skip song
# autoplay next
# choose random
# in terminal option to add song name and URL and playlist


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

mapfile -t playlist < "$FULL_PATH"

# We print the array, pipe it to fzf, and save the choice to a variable
choice=$(printf "%s\n" "${playlist[@]}" | fzf --prompt="Select Starting Song: " --height=10% --reverse)

# Extract the URL (everything after the '|') and play it
# If the user pressed ESC, 'choice' will be empty, so we check first

if [ -z "$choice" ]; then
    echo "No selection. Exiting."
    exit 2
fi

# Find the index of the chosen song in the array

current_index=0
if [[ "$choice" == "Shuffle" ]]; then
   echo "Shuffling playlist" 
   mapfile -t playlist < <(printf "%s\n" "${playlist[@]}" | grep -v "^Shuffle$" | shuf)
else 
    for i in "${!playlist[@]}"; do
       if [[ "${playlist[$i]}" == "$choice" ]]; then
           current_index=$i
           break
       fi
    done
fi

while [[ $current_index -lt ${#playlist[@]} ]]; do
    
    current_line="${playlist[$current_index]}"
    
    # Extract URL (everything after '|') and trim whitespace
    url=$(echo "$current_line" | cut -d'|' -f2 | xargs)
    song_name=$(echo "$current_line" | cut -d'|' -f1 | xargs)

    echo "------------------------------------------"
    echo "Now Playing [$((current_index + 1))/${#playlist[@]}]: $song_name"
    echo "------------------------------------------"

    mpv --no-video\
        --msg-level=ffmpeg=error,demuxer=error\
        --input-conf=<(printf "n quit 0\nq quit 123\n")\
        "$url"
     
    # Check if mpv was interrupted (e.g., you closed the terminal or hit Ctrl+C twice)
    if [[ $? -ne 0 ]]; then
        echo "Playback stopped."
        break
    fi

    # Move to the next song
    ((current_index++))
done

echo "Reached the end of the playlist."
