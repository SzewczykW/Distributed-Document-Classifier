from pathlib import Path
from faker import Faker
import random

# Setup paths
curdir = Path.cwd()
example_dir = curdir / "example"
input_dir = example_dir / "input"
dict_path = example_dir / "dict.txt"

# Create directories
input_dir.mkdir(parents=True, exist_ok=True)

# Dictionary words
dict_words = ["lorem", "ipsum", "dolor", "amet", "elit", "sed", "do", "eiusmod", "tempor", "incididunt"]
with dict_path.open("w") as f:
    for word in dict_words:
        f.write(f"{word}\n")

# Generate large documents
faker = Faker()
num_docs = 8
doc_size_kb = 512
doc_paths = []

for i in range(num_docs):
    doc_path = input_dir / f"doc{i+1}.txt"
    doc_paths.append(doc_path)
    with doc_path.open("w") as f:
        while f.tell() < doc_size_kb * 1024:
            sentence = faker.text(max_nb_chars=200)
            if random.random() < 0.3:
                sentence += " " + random.choice(dict_words)
            f.write(sentence + "\n")

str(dict_path), [str(p) for p in doc_paths]

