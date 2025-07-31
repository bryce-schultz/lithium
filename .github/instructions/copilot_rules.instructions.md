---
applyTo: '**/*.{li}'
---
Lithium programming language does not support // style comments or multiline comments.
Please use # for single-line comments.
More information about lithium, it's syntax and semantics can be found in the LANGUAGE_SPECIFICATION.md file.

---
applyTo: '**/*.{cpp,h}'
---
Follow these C++ coding style guidelines:
- Opening braces `{` should always be placed on new lines
- Use consistent indentation (4 spaces)
- Prefer explicit variable types over auto when clarity is important
- Use meaningful variable and function names
- Always include proper error handling

---
applyTo: '**/*.{md}'
---
For Markdown files:
- Use proper heading hierarchy (don't skip heading levels)
- Include code blocks with appropriate language tags
- Keep line lengths reasonable for readability
- Use consistent bullet point formatting

---
applyTo: '**/*'
---
General coding standards for all files:
- Do not leave trailing whitespace at the end of lines
- Ensure files end with a single newline character
- Use consistent line endings (LF for Unix/Linux systems)
- If you add new lithium tests for any reason add them to the proper place in the test directory. If they are temporary or for debugging, add tmp to the end of the test name then remove them when they are no longer needed.
- Do not suggest code that has been deleted or is no longer relevant.