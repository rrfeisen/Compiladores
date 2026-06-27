#ifndef _FRAME_ACESSO_HPP_
#define _FRAME_ACESSO_HPP_
using namespace std;

// Classe base abstrata para os dois modos de acesso a uma variável:
//  - FrameAcessoNoFrame: variável vive no frame da função (memória)
//  - FrameAcessoTemp:    variável vive em um pseudo-registrador
class FrameAcesso {
public:
  virtual ~FrameAcesso() {}
};

#endif
