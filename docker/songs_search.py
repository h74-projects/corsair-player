from flask import Flask, request, jsonify
import requests
from youtubesearchpython  import VideosSearch


app = Flask(__name__)

# Replace with your actual ChatGPT API key
CHATGPT_API_KEY = "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"

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
    
    headers = {
        'Content-Type': 'application/json',
        'Authorization': f'Bearer {CHATGPT_API_KEY}'
    }
    
    payload = {
        'model': 'gpt-3.5-turbo',
        'messages': [{'role': 'system', 'content': 'You are a helpful assistant.'},
                     {'role': 'user', 'content': message}]
    }
    
    response = requests.post(CHATGPT_API_URL, json=payload, headers=headers)
    chat_response = response.json()['choices'][0]['message']['content']
    
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
    
    
if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
