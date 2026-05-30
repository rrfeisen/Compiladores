# ============================================================
# Makefile – IconLexer
# Execute este Makefile de dentro da pasta IconLexer/
# Compila com javac diretamente, sem precisar do Maven.
# ============================================================

SRC_DIR     := src/main/java
CLASSES_DIR := target/classes

INS_DIR     := ins
OUTS_DIR    := outs

ICN_FILES   := $(wildcard $(INS_DIR)/*.icn)
TOKEN_FILES := $(patsubst $(INS_DIR)/%.icn, $(OUTS_DIR)/%.tokens, $(ICN_FILES))

# ─── Alvo padrão ─────────────────────────────────────────────
.PHONY: all clean compile tokens

all: compile tokens

# ─── Compilação com javac ─────────────────────────────────────
compile:
	mkdir -p $(CLASSES_DIR)
	javac -d $(CLASSES_DIR) $(SRC_DIR)/*.java

# ─── Cria pasta de saída se não existir ──────────────────────
$(OUTS_DIR):
	mkdir -p $(OUTS_DIR)

# ─── Gera .tokens para todos os .icn em ins/ ─────────────────
tokens: $(OUTS_DIR) $(TOKEN_FILES)

# Regra genérica: ins/foo.icn → outs/foo.tokens
$(OUTS_DIR)/%.tokens: $(INS_DIR)/%.icn
	@echo "Lexando: $< → $@"
	java -cp $(CLASSES_DIR) IconLexer $< > $@ 2>&1

# ─── Limpa tudo ──────────────────────────────────────────────
clean:
	rm -rf $(CLASSES_DIR)
	rm -rf $(OUTS_DIR)