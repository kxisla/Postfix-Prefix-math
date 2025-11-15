#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_TOKENS  1024 
#define MAX_LINE    8192 

/* ====== Ngăn xếp cho số thực ====== */
typedef struct {
    double data[MAX_TOKENS];
    int top;
} DoubleStack;

/* Đẩy (push) một giá trị lên ngăn xếp */
static void stack_push(DoubleStack *stack, double value) {
    if (stack->top >= MAX_TOKENS - 1) {
        printf("Loi ngan xep day.\n");
        exit(1);
    }
    stack->data[++(stack->top)] = value;
}

/* Lấy (pop) một giá trị khỏi ngăn xếp */
static double stack_pop(DoubleStack *stack) {
    if (stack->top < 0) {
        printf("Loi khong du toan hang.\n");
        exit(1);
    }
    return stack->data[(stack->top)--];
}

/* ====== Kiểm tra token ====== */
static int is_operator(const char *token) {
    /* Toán tử 1 ký tự: + - * / ^ */
    return (strlen(token) == 1) && (strchr("+-*/^", token[0]) != NULL);
}

static int is_number_token(const char *token) {
    /* Chấp nhận dạng số thực bởi strtod (có thể có dấu, phần thập phân, số mũ) */
    if (token == NULL || *token == '\0') return 0;
    char *endptr;
    (void)strtod(token, &endptr);
    return *endptr == '\0';
}

/* Tính kết quả a (op) b */
static double apply_operator(char op, double a, double b) {
    switch (op) {
        case '+': 
			return a + b;
        case '-':
			return a - b;
        case '*': 
			return a * b;
        case '/':
            if (b == 0.0) {
                printf("Loi chia cho 0.\n");
                exit(1);
            }
            return a / b;
        case '^': return pow(a, b);
        default:
            printf("Loi toan tu khong hop le '%c'.\n");
            exit(1);
    }
}

/* ====== Tách chuỗi thành token theo khoảng trắng ====== */
static int tokenize_line(char *line, char *tokens[], int max_tokens) {
    int count = 0;
    for (char *p = strtok(line, " \t\r\n"); p && count < max_tokens; p = strtok(NULL, " \t\r\n")) {
        tokens[count++] = p;
    }
    return count;
}

/* ====== Phân loại nhanh: nếu token đầu là toán tử => coi là prefix,
         ngược lại => coi là postfix. */
static int looks_like_prefix(char *tokens[], int token_count) {
    return token_count > 0 && is_operator(tokens[0]);
}

/* ====== Đánh giá biểu thức postfix ======
   Duyệt trái -> phải:
   - Gặp số: đẩy vào stack
   - Gặp toán tử: pop 2 số (a, b), tính a op b, đẩy kết quả
*/
static double evaluate_postfix(char *tokens[], int token_count) {
    DoubleStack stack = {.top = -1};

    for (int i = 0; i < token_count; i++) {
        const char *t = tokens[i];

        if (is_number_token(t)) {
            double value = strtod(t, NULL);
            stack_push(&stack, value);
        } else if (is_operator(t)) {
            double b = stack_pop(&stack);
            double a = stack_pop(&stack);
            double result = apply_operator(t[0], a, b);
            stack_push(&stack, result);
        } else {
            printf("Loi token khong hop le'%s'.\n");
            exit(1);
        }
    }

    if (stack.top != 0) {
        printf("Loi bieu thuc Postfix khong hop le.\n");
        exit(1);
    }

    return stack_pop(&stack);
}

/* ====== Đánh giá biểu thức prefix ======
   Duyệt phải -> trái:
   - Gặp số: đẩy vào stack
   - Gặp toán tử: pop 2 số (a, b) theo đúng thứ tự a trước b, tính a op b, đẩy kết quả
*/
static double evaluate_prefix(char *tokens[], int token_count) {
    DoubleStack stack = {.top = -1};

    for (int i = token_count - 1; i >= 0; i--) {
        const char *t = tokens[i];

        if (is_number_token(t)) {
            double value = strtod(t, NULL);
            stack_push(&stack, value);
        } else if (is_operator(t)) {
            double a = stack_pop(&stack);
            double b = stack_pop(&stack);
            double result = apply_operator(t[0], a, b);
            stack_push(&stack, result);
        } else {
            printf("Loi: Token khong hop le'%s'.\n", t);
            exit(1);
        }
    }

    if (stack.top != 0) {
        printf("Loi bieu thuc prefix khong hop le.\n");
        exit(1);
    }

    return stack_pop(&stack);
}

/* ====== In kết quả: nếu là số nguyên thì in không có phần thập phân ====== */
static void print_number_pretty(double value) {
    double rounded = round(value);
    if (fabs(value - rounded) < 1e-9) {
        printf("Ket qua: %.0f\n", rounded);
    } else {
        printf("Ket qua: %.10g\n", value);
    }
}

/* ====== Chương trình chính ====== */
int main(void) {
    char input_line[MAX_LINE];

    printf("Nhap mot bieu thuc Prefix hoac Postfix (Cach nhau bang space)\n");
    printf("> ");

    if (!fgets(input_line, sizeof(input_line), stdin)) {
        return 0;
    }

    char *tokens[MAX_TOKENS];
    int token_count = tokenize_line(input_line, tokens, MAX_TOKENS);

    if (token_count == 0) {
        printf("Khong co token nao.\n");
        return 1;
    }

    double result = 0.0;
    if (looks_like_prefix(tokens, token_count)) {
        result = evaluate_prefix(tokens, token_count);
    } else {
        result = evaluate_postfix(tokens, token_count);
    }

    print_number_pretty(result);
    return 0;
}

