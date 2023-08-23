from flask import Flask, request, jsonify
import openai

app = Flask(__name__)

CHATGPT_API_KEY = "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"
CHATGPT_API_URL = "https://api.openai.com/v1/chat/completions"

def chat():
    data = request.json
    if 'message' not in data:
        return jsonify({'error': 'Message not provided'}), 400
    
    message = data['message']
    
    if message.startswith('1'):
        # Extract the message content after the first character
        message_content = message[1:]
        
        prompt = f"User: {message_content}\nAssistant:"
        
        openai.api_key = CHATGPT_API_KEY
        response = openai.Completion.create(
            engine="text-davinci-003", 
            prompt=prompt,
            max_tokens=500
        )
        
        chat_response = response.choices[0].text.strip()
   
    else:
        chat_response = "Invalid input: Message must start with '1'"
    
    return jsonify({'response': chat_response})

@app.route('/chat', methods=['POST'])
def chat_route():
    return chat()

if __name__ == '__main__':
    app.run(debug=True)

