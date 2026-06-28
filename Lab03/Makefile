CXX ?= g++
CXXFLAGS ?= -std=c++11 -Wall -Wextra -Wno-sign-compare
GRAM ?= 9

TARGET := compilador
BUILD_DIR := build
GRAM_DIR := src/src-gram$(GRAM)
GRAM_FILE ?= gramatica-$(GRAM)/gramatica-$(GRAM).site
TABELA_FILE ?= gramatica-$(GRAM)/tabela_lr1.conf

TOKENS ?=
PARAMS ?=
SAIDA ?=

CORE_SRCS := \
	src/sol.cpp \
	src/Arvore.cpp \
	src/Gramatica.cpp \
	src/Parser.cpp \
	src/TabelaLR1.cpp \
	src/debug-util.cpp \
	src/matriz-util.cpp

GRAM_SRCS := $(wildcard $(GRAM_DIR)/*.cpp)
SRCS := $(CORE_SRCS) $(GRAM_SRCS)
OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(GRAM_DIR) -Isrc -MMD -MP -c $< -o $@

run: $(TARGET)
	@if [ ! -d ins ]; then \
		echo "Diretorio ins/ nao encontrado"; \
		exit 1; \
	fi
	@for tokens in ins/*.tokens; do \
		base=$$(basename $$tokens .tokens); \
		params=ins/$$base.params; \
		saida=ins/$$base.saida; \
		if [ ! -f $$params ]; then \
			echo "AVISO: $$params nao encontrado"; \
			continue; \
		fi; \
		echo "=== Executando $$base ==="; \
		./$(TARGET) $(GRAM_FILE) $(TABELA_FILE) $$params < $$tokens > $$saida; \
		echo "Resultado salvo em $$saida"; \
		echo ""; \
	done

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

-include $(DEPS)