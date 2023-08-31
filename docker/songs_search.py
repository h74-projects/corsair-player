from flask import Flask, request, jsonify
import openai
from youtubesearchpython  import VideosSearch
import lyricsgenius


app = Flask(__name__)

# Replace with your actual ChatGPT API key
#Put in local envirment
openai.api_key = "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"

# ChatGPT API endpoint
CHATGPT_API_URL = "https://api.openai.com/v1/chat/completions"

@app.route('/greet')
def print_hey():
    return "hey user"

@app.route('/chat', methods=['POST'])
def chat():
    data = request.json
    if 'message' not in data:
        return jsonify({'error': 'Message not provided'}), 400
    
    message = data['message']
    
    prompt = f"User: {message}\nAssistant:"
    
    response = openai.Completion.create(
        engine="text-davinci-003", 
        prompt=prompt,
        max_tokens=700  # Adjust as needed
    )
    
    chat_response = response.choices[0].text.strip()
    
    return jsonify({'response': chat_response})


@app.route('/search', methods=['POST'])
def search_youtube():
    data = request.json
    queries = data.get('q', [])
    
    if not queries:
        return jsonify({'error': 'No queries provided'}), 400
    
    all_links = []
    
    for query in queries:
        results = VideosSearch(query, limit=1).result()
        
        if 'result' in results and len(results['result']) > 0:
            video_id = results['result'][0]['id']
            video_link = f"https://www.youtube.com/watch?v={video_id}"
            all_links.append(video_link)
        else:
            all_links.append(None)
    
    return jsonify({'video_links': all_links})

GENIUS_ACCESS_TOKEN = "3KaaL4GnPKjv-B4D6Ehsqo5unXXKxVzzFVNpiAP7VQ3Vxrqg1hULF4dHDR111Wm0"

@app.route('/lyrics', methods=['GET'])
def get_lyrics():
    query = request.args.get('query')
    
    if not query:
        return jsonify({"error": "Missing 'query' parameter"}), 400

    genius = lyricsgenius.Genius(GENIUS_ACCESS_TOKEN)
    song = genius.search_song(query)
    
    if song:
        return jsonify({"lyrics": song.lyrics})
    else:
         return jsonify({"lyrics": "Sorry - Lyrics not found"})
    
if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
