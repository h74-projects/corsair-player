import requests

# Define the API endpoint URL
api_url = "http://127.0.0.1:5000/chat"  # Replace with the actual URL

# Define the message you want to send
user_message = "1top ten songs of the 70s"

# Create the request payload
payload = {"message": user_message}

try:
    # Send the POST request
    response = requests.post(api_url, json=payload)
    response.raise_for_status()  # Raise an exception for HTTP errors
    
    # Save the chatbot's response to a file
    with open("chatbot_response.txt", "w") as file:
        file.write(response.json()["response"])

    print("Chatbot response saved to chatbot_response.txt")
except requests.exceptions.RequestException as e:
    print("Error:", e)
