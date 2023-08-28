import lyricsgenius
import json

GENIUS_ACCESS_TOKEN = "3KaaL4GnPKjv-B4D6Ehsqo5unXXKxVzzFVNpiAP7VQ3Vxrqg1hULF4dHDR111Wm0"

def get_lyrics(artist, song_title):
    genius = lyricsgenius.Genius(GENIUS_ACCESS_TOKEN)
    song = genius.search_song(song_title, artist)
    
    if song:
        lyrics = song.lyrics
        result = {
            "artist": artist,
            "song_title": song_title,
            "lyrics": lyrics
        }
        output_file = "lyrics.json"
        with open(output_file, "w") as json_file:
            json.dump(result, json_file, ensure_ascii=False, indent=4)
            print(f"Lyrics data saved to {output_file}")
    
    else: 
        print("error: Lyrics not found for this song.") # exeption??
    
artist_name = "Leonard Cohen"
song_name = "Hallelujah"
lyrics_data = get_lyrics(artist_name, song_name)
