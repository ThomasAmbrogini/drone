# Drone Project: Claude Guidelines

## General Behaviour

### Do Not Build or Run Commands
Do not run build commands (e.g. `make html`, `sphinx-build`) or any other
shell commands unless the user explicitly asks for it.

### Do Not Edit Files Unsolicited
Do not apply changes to any file unless the user explicitly asks for it.
Propose changes in the response as text or a diff; wait for the user to
request that the edit is applied.

## Writing Style

### No Em-Dashes
Do not use em-dashes (`—`) anywhere in documentation, comments, or prose.
Use proper punctuation instead:

| Instead of | Use |
|------------|-----|
| `ESC — Technical Reference` | `ESC: Technical Reference` (colon for titles/labels) |
| `servo tester — a small device` | `servo tester, a small device` (comma for appositives) |
| `sends a signal back — it does not...` | `sends a signal back; it does not...` (semicolon for related clauses) |
| `never short leads — fire risk` | `never short leads (fire risk)` (parentheses for asides) |
| `Step 1 — Apply signal` | `Step 1: Apply signal` (colon for steps) |
| `high throttle — it can damage` | `high throttle, as it can damage` (conjunction for causal links) |
