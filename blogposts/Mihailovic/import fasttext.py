import fasttext
import csv

# Written by Milan Mihailovic with the assistance of ChatGPT

# Load the pre-trained FastText model
model = fasttext.load_model('cc.en.300.bin')

# Open the file in read mode and a CSV file in write mode
with open('/mnt/c/Users/mihai/Desktop/CSE 491/blogpost/src/sentences.txt', 'r') as file, open('/mnt/c/Users/mihai/Desktop/CSE 491/blogpost/src/vectors.csv', 'w', newline='') as out_file:
    writer = csv.writer(out_file)
    for sentence in file:
        sentence = sentence.strip()
        vector = model.get_sentence_vector(sentence).tolist()
        writer.writerow(vector)