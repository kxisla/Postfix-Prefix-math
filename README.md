- DoubleStack STRUCTURE:
data: double array of size MAX_TOKENS
top: integer, store top element position

- FUNCTION stack_push(stack, value):
IF stack.top >= MAX_TOKENS - 1 THEN
PRINT "Stack full error"
END OF PROGRAM
END IF

INCREASE stack.top by 1
ASSIGN stack.data[stack.top] = value
END OF FUNCTION

- FUNCTION stack_pop(stack) RETURN double:
IF stack.top < 0 THEN
PRINT "Insufficient operand error"
END OF PROGRAM
END IF

value = stack.data[stack.top]
DECREASE stack.top by 1
RETURN TO value
END OF FUNCTION

- FUNCTION is_operator(token) RETURNS bool:
IF token length ≠ 1 THEN
RETURNS FALSE
END IF

character = token[0]

IF character is one of the characters '+', '-', '*', '/', '^' THEN
RETURNS TRUE
OTHERWISE
RETURNS FALSE
END IF
END OF FUNCTION

- FUNCTION is_number_token(token) RETURNS bool:
IF token is NULL OR token is empty THEN
RETURNS FALSE
END IF

CALL strtod(token, endptr)
// strtod will read the first part of the string as a number, endptr points to the position right after the number

IF the character at position endptr is the character that ends the string '\0' THEN
// That means the entire string is a valid number
RETURNS TRUE
OTHERWISE
RETURNS FALSE
END IF
END OF FUNCTION

- FUNCTION apply_operator(op, a, b) RETURNS double:
IF op == '+' THEN
RETURNS a + b
OTHERWISE IF op == '-' THEN
RETURNS a - b
OTHERWISE IF op == '*' THEN
RETURNS a * b
OTHERWISE AGAIN IF op == '/' THEN
IF b == 0 THEN
PRINT "Division by zero error"
END OF PROGRAM
END IF
RETURN a / b
OTHERWISE IF op == '^' THEN
RETURN a to the power of b
OTHERWISE
PRINT "Invalid operator error"
END OF PROGRAM
END IF
END OF FUNCTION
- FUNCTION tokenize_line(line, tokens, max_tokens) RETURN integer:
count = 0

p = first token after line is split by delimiters (space, tab, newline)

REPEAT WHEN p is not NULL AND count < max_tokens:
tokens[count] = p
INCREMENT count by 1

p = next token (continue cutting the same line)
END OF ITEM

RETURN count
END OF FUNCTION

- FUNCTION looks_like_prefix(tokens, token_count) RETURN bool:
IF token_count > 0 AND first token tokens[0] is operator (is_operator(tokens[0]) == TRUE) THEN
RETURN TRUE // looks like prefix
OTHERWISE
RETURN FALSE // consider postfix
END IF
END OF FUNCTION

- FUNCTION evaluate_postfix(tokens, token_count) RETURN double:
Create a stack of type DoubleStack with top = -1

FOR i running from 0 to token_count - 1:
t = tokens[i]

IF t is a number (is_number_token(t) == TRUE) THEN
value = convert t to a float (strtod)
push value onto the stack
OTHERWISE IF t is an operator (is_operator(t) == TRUE) THEN
b = pop from stack
a = pop from stack
result = apply_operator(operator t[0], a, b)
push result onto the stack
OTHERWISE
PRINT "Invalid token error"
END OF PROGRAM
END IF
END OF LOOP

IF stack.top != 0 THEN
PRINT "Invalid postfix expression error" (due to remaining or missing operands)
END OF PROGRAM
END IF

RESULT = pop from stack
RETURN RESULT
END OF FUNCTION

- FUNCTION evaluate_prefix(tokens, token_count) RETURN double:
Create a stack of type DoubleStack with top = -1

FOR i run from token_count - 1 to 0 (traverse BACKWARDS):
t = tokens[i]

IF t is a number (is_number_token(t) == TRUE) THEN
value = convert t to a float (strtod)
push value onto stack
OTHERWISE IF t is an operator (is_operator(t) == TRUE) THEN
a = pop from stack // note: a is the left operand
b = pop from stack // b is the right operand
result = apply_operator(operator t[0], a, b)
push result onto stack
COUNTER
PRINT "Invalid token error"
END OF PROGRAM
END IF
END OF LOOP

IF stack.top != 0 THEN
PRINT "Invalid prefix expression error"
END OF PROGRAM
END IF

RESULT = pop from stack
RETURN RESULT
END OF FUNCTION

- FUNCTION print_number_pretty(value):
rounded = round value to nearest integer

IF |value - rounded| < 1e-9 THEN
// treat as integer
PRINT "Result: " + rounded (print without decimal part)
VERSUS
PRINT "Result: " + value (print as real number with certain precision)
END IF
END OF FUNCTION

- FUNCTION main:
Create string variable input_line with size MAX_LINE

PRINT "Enter a Prefix or Postfix expression (separated by space)"
IN "> "

Read a line from stdin into input_line
IF cannot read (EOF) THEN
RETURN 0
END IF

Create array of tokens with size MAX_TOKENS

token_count = tokenize_line(input_line, tokens, MAX_TOKENS)

IF token_count == 0 THEN
PRINT "No which token."
RETURN 1
ALL IF
