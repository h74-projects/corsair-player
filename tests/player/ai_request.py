import openai
import sys

def generate_response(request):
    openai.api_key = "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"

    request += "\n format the output as: Song_Name-Artists_Names-Year-Genre"


    response = openai.Completion.create(
        engine="text-davinci-003",
        prompt=request,
        max_tokens=700
    )
    return response.choices[0].text.strip()

if __name__ == "__main__":
    varibale = sys.argv[1]
    print(generate_response(varibale))

   




