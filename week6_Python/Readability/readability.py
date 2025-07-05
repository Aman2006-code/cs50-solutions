from cs50 import get_string

user_input = get_string("Text: ")

letter_count = 0
word_count = 1
sentence_count = 0

for letter in user_input:

    if letter.isalpha():
        letter_count += 1

    elif letter == ' ':
        word_count += 1

    elif letter in ['.','?','!']:
        sentence_count += 1


L = (letter_count / word_count) * 100
S = (sentence_count / word_count) * 100

index = round(0.0588 * L - 0.296 * S - 15.8)

if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print(f"Grade {index}")

