CLI Radio for Debian 13

A lightweight, terminal-based YouTube audio streamer designed to maximize battery life and minimize system resource usage. This tool allows you to organize your music into playlists (categories) and stream them via the terminal without a web browser.
 Features

    Ultra-low Power Consumption: Saves ~3-4 Watts compared to streaming in Brave/Chrome.

    Nested Menus: Choose by Genre/Playlist first, then by Song.

    Fuzzy Search: Powered by fzf for lightning-fast selection.

    Background Playback: Audio-only streaming via mpv.

 Dependencies

You need three core tools to run this script. Install them via the Debian repositories:
Bash

sudo apt update
sudo apt install fzf mpv yt-dlp

    fzf: The interactive finder for your menus.

    mpv: The versatile media player that handles the audio stream.

    yt-dlp: The engine that extracts audio streams from YouTube URLs.

 Installation

    Create the script file:
    Bash

sudo nano /usr/local/bin/radio

Paste the script code (ensure your BASE_DIR points to your playlists folder).

Important: set your OWN BASE_DIR!

Make it executable:
Bash

    sudo chmod +x /usr/local/bin/radio

 Project Structure

The script expects your playlists to be stored in a specific directory. By default:
~/playlists/

Example structure:
Plaintext

playlists/
├── Jazz.txt
├── Lofi.txt
└── Rock.txt

 How to Add Music

Music is stored in simple .txt files within your playlists folder.

    Create/Open a playlist file:
    Bash

    nano ~/playlists/MyFavorites.txt

    Add songs using this exact format:
    Name of Song | URL

    Example:
    Masayoshi Takanaka - All Of Me | https://www.youtube.com/watch?v=xxxxxxxx

    Save and Exit. The script will automatically detect the new file and the new songs.

 How to Run

Simply open your terminal and type:
Bash

radio

    Use the Arrow Keys or type to search.

    Press Enter to select.

    Press ESC to cancel.

    While music is playing, press Q in the terminal to stop the music and exit.
