import numpy as np
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.metrics.pairwise import cosine_similarity

def sort_sentences_by_similarity(sentences):
    # Initialize the TF-IDF vectorizer
    vectorizer = TfidfVectorizer()
    
    # Transform sentences into TF-IDF vectors
    tfidf_matrix = vectorizer.fit_transform(sentences)
    print('vectorized')
    
    # Compute cosine similarity matrix
    cosine_sim_matrix = cosine_similarity(tfidf_matrix, tfidf_matrix)
    print('got consine similarity')
    
    # For sorting, we will use the sum of cosine similarities for each sentence
    similarity_sums = cosine_sim_matrix.sum(axis=1)
    print('sum')
    
    # Sort indices based on the similarity sums
    sorted_indices = np.argsort(-similarity_sums)
    print('got indices')
    
    # Sort sentences based on the sorted indices
    sorted_sentences = [sentences[i] for i in sorted_indices]
    print('sort completed')
    
    # Return the sorted sentences and the sorted indices
    return sorted_sentences, sorted_indices

def restore_original_order(sentences, sorted_indices):
    print(sorted_indices)
    # Create a list of (original index, sentence) pairs
    
    # Extract sentences in their original order
    restored_sentences = sentences.copy()
    for org_index, index in enumerate(sorted_indices):
        restored_sentences[index] = sentences[org_index]
    
    return restored_sentences

import xml.etree.ElementTree as ET
import re

def extract_sentences(enwik9_file):
    sentences = []
    current_sentence = b''
    with open(enwik9_file, 'rb') as file:
        for line in file:
            if line.strip().endswith(b"."):
                current_sentence = current_sentence + line
                sentences.append(current_sentence)
                current_sentence = b''

            else:
                current_sentence = current_sentence + line
    if current_sentence != b'':
        sentences.append(current_sentence)
    return sentences


def write_sentences_to_file(sentences, output_file, sorted_indices, sortorder_file):
    with open(output_file, 'wb') as out_file:
        for sentence in sentences:
          out_file.write(sentence)

    with open(sortorder_file, 'w', encoding='utf-8') as out_file:
        for index in sorted_indices:
          out_file.write(f'{str(index)}\n')



if __name__ == "__main__":
    enwik9_file = 'enwik9'
    output_file = 'sorted_sentences_enwik9'
    sortorder_file = 'sorted_indices_enwik9.txt'

    # Extract pages
    sentences = extract_sentences(enwik9_file)
    print('total : ', len(sentences), ' sentences')
    
    sorted_sentences, sorted_indices = sort_sentences_by_similarity(sentences[:16436])

    # Reverse the order of pages and write them to the output file
    write_sentences_to_file(sorted_sentences, output_file, sorted_indices, sortorder_file)

# # Example usage
# sentences = [
#     "The quick brown fox jumps over the lazy dog.",
#     "A fast, dark-colored fox leaps over a sleepy dog.",
#     "A fox is quick and brown.",
#     "The dog is lazy and brown."
# ]

# # Sort sentences by similarity
# sorted_sentences, sorted_indices = sort_sentences_by_similarity(sentences)
# print("Sorted Sentences:")
# for sentence in sorted_sentences:
#     print(sentence)

# # Restore the original order
# restored_sentences = restore_original_order(sorted_sentences, sorted_indices)
# print("\nRestored Original Order:")
# for sentence in restored_sentences:
#     print(sentence)
