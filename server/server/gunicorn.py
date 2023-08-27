from flask import Flask, request, jsonify
import openai
#from youtube_search import YoutubeSearch

app = Flask(__name__)

# YouTube Search Endpoint
@app.route('/search', methods=['GET'])
def search_youtube():
    query = request.args.get('q')
    
    if not query:
        return jsonify({'error': 'Query parameter "q" is missing'}), 400
    
#    results = YoutubeSearch(query, max_results=5).to_dict()
    videos = []
    
    for result in results:
        video_info = {
            'title': result['title'],
            'link': f"https://www.youtube.com/watch?v={result['id']}",
            'duration': result['duration'],
            'views': result['views'],
            'published_at': result['publish_time']
        }
        videos.append(video_info)
    
    return jsonify({'videos': videos})

# Chat Endpoint
@app.route('/chat', methods=['POST'])
def chat_route():
    data = request.json
    if 'message' not in data:
        return jsonify({'error': 'Message not provided'}), 400
    
    message = data['message']
    
    if message.startswith('1'):
        # Extract the message content after the first character
        message_content = message[1:]
        
        prompt = f"User: {message_content}\nAssistant:"

        openai.api_key = "YOUR_OPENAI_API_KEY"
        response = openai.Completion.create(
            engine="text-davinci-003", 
            prompt=prompt,
            max_tokens=50
        )
        
        chat_response = response.choices[0].text.strip()
    elif message.startswith('2'):
        return search_youtube()
    else:
        chat_response = "Invalid input: Message must start with '1' or '2'"
    
    return jsonify({'response': chat_response})

if __name__ == '__main__':
    app.run(debug=True)
