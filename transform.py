# Open the binary file for reading
with open("modified_enwik9", 'rb') as f:
    enwik9 = f.read()
print("File read")

# Create a dictionary of replacements
replacements = {
    b"<page>": b"",
    b"</page>": b"",
    b"<title>": b"",
    b"</title>": b"",
    b"<id>": b"",
    b"</id>": b"",
    b"<ip>": b"",
    b"</ip>": b"",
    b"<revision>": b"",
    b"</revision>": b"",
    b"<timestamp>": b"",
    b"</timestamp>": b"",
    b"<contributor>": b"",
    b"</contributor>": b"",
    b"<username>": b"",
    b"</username>": b"",
    b"<comment>": b"",
    b"</comment>": b""
}

# Perform the replacements
e = enwik9
for old, new in replacements.items():
    e = e.replace(old, new)
    print(f"Replaced {old} with {new}")

# Open the binary file for writing
with open("a_org", 'wb') as f:
    f.write(e)
print("File written")
