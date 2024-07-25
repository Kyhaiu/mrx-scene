#include <core/vector.hpp>

namespace core
{

  // ------------------------------------------------------------------------------------------
  // Methods - Vector2
  // ------------------------------------------------------------------------------------------

  /**
   * @brief Cria um vetor com componentes 0.0f
   *
   * @return Vetor 2D (Vector2) com componentes (x, y) 0.0f
   */
  Vector2 Vector2Zero()
  {
    Vector2 result = {0.0f, 0.0f};

    return result;
  }

  /**
   * @brief Cria um vetor com componentes 1.0f
   *
   * @return Vetor 2D (Vector2) com componentes (x, y) 1.0f
   */
  Vector2 Vector2One(void)
  {
    Vector2 result = {1.0f, 1.0f};

    return result;
  }

  /**
   * @brief Calcula o comprimento do vetor
   *
   * @param v Vetor 2D
   * @return float
   */
  float Vector2Length(Vector2 v)
  {
    float result = sqrtf((v.x * v.x) + (v.y * v.y));

    return result;
  }

  /**
   * @brief Verifica se dois vetores são iguais.
   *
   * @param v1 O primeiro vetor
   * @param v2 O segundo vetor
   * @return boolean
   */
  bool Vector2Equals(Vector2 v1, Vector2 v2)
  {

    int result = ((fabsf(v1.x - v2.x)) <= (0.000001f * fmaxf(1.0f, fmaxf(fabsf(v1.x), fabsf(v2.x))))) &&
                 ((fabsf(v1.y - v2.y)) <= (0.000001f * fmaxf(1.0f, fmaxf(fabsf(v2.y), fabsf(v2.y)))));

    return result;
  }

  /**
   * @brief Converte o vetor 2D para um array de floats
   *
   * @param v O vetor 2D
   * @return float2 (array de 2 floats)
   */
  float2 Vector2ToFloat2(const Vector2 v)
  {
    float2 result = {v.x, v.y};

    return result;
  }

  /**
   * @brief Esta função realiza a interpolação linear entre dois vetores 2D.
   *
   * A interpolação linear cria um novo valor entre dois valores originais,
   * com base em um fator de interpolação (amount). Nesse caso, ele cria um
   * novo vetor em algum lugar ao longo da linha reta entre os dois vetores fornecidos.
   *
   * @param v1 O primeiro vetor
   * @param v2 O segundo vetor
   * @param amount A quantidade para interpolar
   * @return Vector2
   */
  Vector2 Vector2Lerp(Vector2 v1, Vector2 v2, float amount)
  {
    Vector2 result = {0};

    result.x = v1.x + amount * (v2.x - v1.x);
    result.y = v1.y + amount * (v2.y - v1.y);

    return result;
  }

  /**
   * @brief Limita um vetor 2D dentro de um intervalo definido.
   *
   * Esta função garante que as coordenadas X e Y do vetor `v` estejam dentro do intervalo definido pelos vetores `min` e `max`.
   * Se um valor de `v` estiver fora do intervalo, ele será ajustado para o valor limite mais próximo.
   *
   * @param v O vetor 2D a ser limitado.
   * @param min O vetor 2D definindo o limite inferior do intervalo.
   * @param max O vetor 2D definindo o limite superior do intervalo.
   * @return Vector2 O vetor `v` limitado dentro do intervalo definido por `min` e `max`.
   */
  Vector2 Vector2Clamp(Vector2 v, Vector2 min, Vector2 max)
  {
    Vector2 result = {0};

    result.x = fminf(max.x, fmaxf(min.x, v.x));
    result.y = fminf(max.y, fmaxf(min.y, v.y));

    return result;
  }

  /**
   * @brief Limita a magnitude de um vetor 2D a um intervalo especificado.
   *
   * Esta função limita a magnitude (comprimento) do vetor `v` para estar dentro do intervalo definido por `min` e `max`.
   * A direção do vetor é preservada, e a magnitude é ajustada para ficar dentro do intervalo.
   *
   * Se a magnitude original de `v` for menor que `min`, o vetor será escalado para ter magnitude `min`.
   * Se a magnitude for maior que `max`, o vetor será escalado para ter magnitude `max`.
   *
   * @param v O vetor 2D a ser limitado.
   * @param min O valor mínimo permitido para a magnitude do vetor.
   * @param max O valor máximo permitido para a magnitude do vetor.
   * @return Vector2 O vetor `v` com sua magnitude limitada pelo intervalo [min, max].
   */
  Vector2 Vector2ClampValue(Vector2 v, float min, float max)
  {
    Vector2 result = v;

    float length = (v.x * v.x) + (v.y * v.y);

    if (length > 0.0f)
    {
      length = sqrtf(length);

      float scale = 1; // By default, 1 as the neutral element.
      if (length < min)
      {
        scale = min / length;
      }
      else if (length > max)
      {
        scale = max / length;
      }

      result.x = v.x * scale;
      result.y = v.y * scale;
    }

    return result;
  }

  // ------------------------------------------------------------------------------------------
  // Methods - Vector3
  // ------------------------------------------------------------------------------------------

  /**
   * @brief Retorna um vetor 3D inicializado com zeros.
   *
   * Esta função cria e retorna um novo vetor 3D onde todas as componentes (x, y e z) são inicializadas com o valor 0.0f.
   *
   * @return Vector3 Um novo vetor 3D com todas as componentes iguais a 0.0f.
   */
  Vector3 Vector3Zero(void)
  {
    Vector3 result = {0.0f, 0.0f, 0.0f};

    return result;
  }

  /**
   * @brief Retorna um vetor 3D inicializado com ums.
   *
   * Esta função cria e retorna um novo vetor 3D onde todas as componentes (x, y e z) são inicializadas com o valor 1.0f.
   *
   * @return Vector3 Um novo vetor 3D com todas as componentes iguais a 1.0f.
   */
  Vector3 Vector3One(void)
  {
    Vector3 result = {1.0f, 1.0f, 1.0f};

    return result;
  }

  /**
   * @brief Calcula a magnitude (comprimento) de um vetor 3D.
   *
   * Esta função calcula a magnitude (comprimento) do vetor `v`. A magnitude representa o
   * comprimento da linha reta que vai da origem até o ponto final do vetor.
   *
   * @param v O vetor 3D para o qual a magnitude deve ser calculada.
   * @return float A magnitude do vetor `v`.
   */
  float Vector3Length(Vector3 v)
  {
    float result = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));

    return result;
  }

  /**
   * @brief Verifica se dois vetores 3D são iguais.
   *
   * Esta função compara os componentes dos vetores `v1` e `v2` e verifica se eles são iguais.
   * A comparação é feita com uma tolerância de 0.000001f para lidar com erros de arredondamento.
   *
   * @param v1 O primeiro vetor 3D a ser comparado.
   * @param v2 O segundo vetor 3D a ser comparado.
   * @return boolean Verdadeiro se os vetores forem iguais, falso caso contrário.
   */
  bool Vector3Equals(Vector3 v1, Vector3 v2)
  {

    int result = ((fabsf(v1.x - v2.x)) <= (0.000001f * fmaxf(1.0f, fmaxf(fabsf(v1.x), fabsf(v2.x))))) &&
                 ((fabsf(v1.y - v2.y)) <= (0.000001f * fmaxf(1.0f, fmaxf(fabsf(v2.y), fabsf(v2.y))))) &&
                 ((fabsf(v1.z - v2.z)) <= (0.000001f * fmaxf(1.0f, fmaxf(fabsf(v2.z), fabsf(v2.z)))));

    return result;
  }

  /**
   * @brief Converte o vetor 3D para um array de floats
   *
   * @param v O vetor 3D
   * @return float3 (array de 3 floats)
   */
  float3 Vector3ToFloat3(Vector3 v)
  {
    float3 result = {v.x, v.y, v.z};

    return result;
  }

  /**
   * @brief Calcula a interpolação linear entre dois vetores 3D.
   *
   * A interpolação linear cria um novo valor entre dois valores originais,
   * com base em um fator de interpolação (amount). Nesse caso, ele cria um
   * novo vetor em algum lugar ao longo da linha reta entre os dois vetores fornecidos.
   *
   * @param v1 O primeiro vetor 3D
   * @param v2 O segundo vetor 3D
   * @param amount A quantidade para interpolar
   * @return Vector3
   */
  Vector3 Vector3Lerp(Vector3 v1, Vector3 v2, float amount)
  {
    Vector3 result = {0};

    result.x = v1.x + amount * (v2.x - v1.x);
    result.y = v1.y + amount * (v2.y - v1.y);
    result.z = v1.z + amount * (v2.z - v1.z);

    return result;
  }

  /**
   * @brief Limita um vetor 3D dentro de um intervalo definido.
   *
   * Esta função garante que as coordenadas X, Y e Z do vetor `v` estejam dentro do intervalo definido pelos vetores `min` e `max`.
   * Se um valor de `v` estiver fora do intervalo, ele será ajustado para o valor limite mais próximo.
   *
   * @param v O vetor 3D a ser limitado.
   * @param min O vetor 3D definindo o limite inferior do intervalo.
   * @param max O vetor 3D definindo o limite superior do intervalo.
   * @return Vector3 O vetor `v` limitado dentro do intervalo definido por `min` e `max`.
   */
  Vector3 Vector3Clamp(Vector3 v, Vector3 min, Vector3 max)
  {
    Vector3 result = {0};

    result.x = fminf(max.x, fmaxf(min.x, v.x));
    result.y = fminf(max.y, fmaxf(min.y, v.y));
    result.z = fminf(max.z, fmaxf(min.z, v.z));

    return result;
  }

  /**
   * @brief Limita a magnitude de um vetor 3D a um intervalo especificado.
   *
   * Esta função limita a magnitude (comprimento) do vetor `v` para estar dentro do intervalo definido por `min` e `max`.
   * A direção do vetor é preservada, e a magnitude é ajustada para ficar dentro do intervalo.
   *
   * Se a magnitude original de `v` for menor que `min`, o vetor será escalado para ter magnitude `min`.
   * Se a magnitude for maior que `max`, o vetor será escalado para ter magnitude `max`.
   *
   * @param v O vetor 3D a ser limitado.
   * @param min O valor mínimo permitido para a magnitude do vetor.
   * @param max O valor máximo permitido para a magnitude do vetor.
   * @return Vector3 O vetor `v` com sua magnitude limitada pelo intervalo [min, max].
   */
  Vector3 Vector3ClampValue(Vector3 v, float min, float max)
  {
    Vector3 result = v;

    float length = (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
    if (length > 0.0f)
    {
      length = sqrtf(length);

      float scale = 1; // By default, 1 as the neutral element.
      if (length < min)
      {
        scale = min / length;
      }
      else if (length > max)
      {
        scale = max / length;
      }

      result.x = v.x * scale;
      result.y = v.y * scale;
      result.z = v.z * scale;
    }

    return result;
  }

  // ------------------------------------------------------------------------------------------
  // Methods - Vector4
  // ------------------------------------------------------------------------------------------

  /**
   * @brief Retorna um vetor 4D inicializado com zeros.
   *
   * Esta função cria e retorna um novo vetor 4D onde todas as componentes (x, y, z e w) são inicializadas com o valor 0.0f.
   *
   * @return Vector4 Um novo vetor 4D com todas as componentes iguais a 0.0f.
   */
  Vector4 Vector4Zero(void)
  {
    Vector4 result = {0.0f, 0.0f, 0.0f, 0.0f};

    return result;
  }

  /**
   * @brief Retorna um vetor 4D inicializado com ums.
   *
   * Esta função cria e retorna um novo vetor 4D onde todas as componentes (x, y, z e w) são inicializadas com o valor 1.0f.
   *
   * @return Vector4 Um novo vetor 4D com todas as componentes iguais a 1.0f.
   */
  Vector4 Vector4One(void)
  {
    Vector4 result = {1.0f, 1.0f, 1.0f, 1.0f};

    return result;
  }

  /**
   * @brief Calcula a magnitude (comprimento) de um vetor 4D.
   *
   * Esta função calcula a magnitude (comprimento) do vetor `v`. A magnitude representa o
   * comprimento da linha reta que vai da origem até o ponto final do vetor.
   *
   * @param v O vetor 4D para o qual a magnitude deve ser calculada.
   * @return float A magnitude do vetor `v`.
   */
  float Vector4Length(const Vector4 v)
  {
    float result = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w));

    return result;
  }

  /**
   * @brief Verifica se dois vetores 4D são iguais.
   *
   * Esta função compara os componentes dos vetores `v1` e `v2` e verifica se eles são iguais.
   * A comparação é feita com uma tolerância de 0.000001f para lidar com erros de arredondamento.
   *
   * @param v1 O primeiro vetor 4D a ser comparado.
   * @param v2 O segundo vetor 4D a ser comparado.
   * @return boolean Verdadeiro se os vetores forem iguais, falso caso contrário.
   */
  bool Vector4Equals(const Vector4 v1, const Vector4 v2)
  {

    int result = ((fabsf(v1.x - v2.x)) <= (0.000001f * fmaxf(1.0f, fmaxf(fabsf(v1.x), fabsf(v2.x)))) &&
                  (fabsf(v1.y - v2.y)) <= (0.000001f * fmaxf(1.0f, fmaxf(fabsf(v2.y), fabsf(v2.y)))) &&
                  (fabsf(v1.z - v2.z)) <= (0.000001f * fmaxf(1.0f, fmaxf(fabsf(v2.z), fabsf(v2.z)))) &&
                  (fabsf(v1.w - v2.w)) <= (0.000001f * fmaxf(1.0f, fmaxf(fabsf(v2.w), fabsf(v2.w)))));

    return result;
  }

  /**
   * @brief Converte o vetor 4D para um array de floats
   *
   * @param v O vetor 4D
   * @return float4 (array de 4 floats)
   */
  float4 Vector4ToFloat4(const Vector4 v)
  {
    float4 result = {{v.x, v.y, v.z, v.w}};

    return result;
  }

  /**
   * @brief Calcula a interpolação linear entre dois vetores 4D.
   *
   * A interpolação linear cria um novo valor entre dois valores originais,
   * com base em um fator de interpolação (amount). Nesse caso, ele cria um
   * novo vetor em algum lugar ao longo da linha reta entre os dois vetores fornecidos.
   *
   * @param v1 O primeiro vetor 4D
   * @param v2 O segundo vetor 4D
   * @param amount A quantidade para interpolar
   * @return Vector4
   */
  Vector4 Vector4Lerp(const Vector4 v1, const Vector4 v2, const float amount)
  {
    Vector4 result = {0};

    result.x = v1.x + amount * (v2.x - v1.x);
    result.y = v1.y + amount * (v2.y - v1.y);
    result.z = v1.z + amount * (v2.z - v1.z);
    result.w = v1.w + amount * (v2.w - v1.w);

    return result;
  }

  /**
   * @brief Limita um vetor 4D dentro de um intervalo definido.
   *
   * Esta função garante que as coordenadas X, Y, Z e W do vetor `v` estejam dentro do intervalo definido pelos vetores `min` e `max`.
   * Se um valor de `v` estiver fora do intervalo, ele será ajustado para o valor limite mais próximo.
   *
   * @param v O vetor 4D a ser limitado.
   * @param min O vetor 4D definindo o limite inferior do intervalo.
   * @param max O vetor 4D definindo o limite superior do intervalo.
   * @return Vector4 O vetor `v` limitado dentro do intervalo definido por `min` e `max`.
   */
  Vector4 Vector4Clamp(const Vector4 v, const Vector4 min, const Vector4 max)
  {
    Vector4 result = {0};

    result.x = fminf(max.x, fmaxf(min.x, v.x));
    result.y = fminf(max.y, fmaxf(min.y, v.y));
    result.z = fminf(max.z, fmaxf(min.z, v.z));
    result.w = fminf(max.w, fmaxf(min.w, v.w));

    return result;
  }

  /**
   * @brief Limita a magnitude de um vetor 4D a um intervalo especificado.
   *
   * Esta função limita a magnitude (comprimento) do vetor `v` para estar dentro do intervalo definido por `min` e `max`.
   * A direção do vetor é preservada, e a magnitude é ajustada para ficar dentro do intervalo.
   *
   * Se a magnitude original de `v` for menor que `min`, o vetor será escalado para ter magnitude `min`.
   * Se a magnitude for maior que `max`, o vetor será escalado para ter magnitude `max`.
   *
   * @param v O vetor 4D a ser limitado.
   * @param min O valor mínimo permitido para a magnitude do vetor.
   * @param max O valor máximo permitido para a magnitude do vetor.
   * @return Vector4 O vetor `v` com sua magnitude limitada pelo intervalo [min, max].
   */
  Vector4 Vector4ClampValue(const Vector4 v, const float min, const float max)
  {
    Vector4 result = v;

    float length = (v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w);
    if (length > 0.0f)
    {
      length = sqrtf(length);

      float scale = 1; // By default, 1 as the neutral element.
      if (length < min)
      {
        scale = min / length;
      }
      else if (length > max)
      {
        scale = max / length;
      }

      result.x = v.x * scale;
      result.y = v.y * scale;
      result.z = v.z * scale;
      result.w = v.w * scale;
    }

    return result;
  }

  // ------------------------------------------------------------------------------------------
  // Methods - Matrix
  // ------------------------------------------------------------------------------------------

  /**
   * @brief Retorna uma matriz 4x4 identidade.
   *
   * Esta função cria e retorna uma matriz 4x4 onde todos os elementos são inicializados com 0.0f, exceto
   * os elementos da diagonal principal, que são inicializados com 1.0f.
   *
   * @return Matrix Uma matriz 4x4 identidade.
   */
  Matrix MatrixIdentity(void)
  {
    Matrix result = {0};

    result.m0 = 1.0f;
    result.m5 = 1.0f;
    result.m10 = 1.0f;
    result.m15 = 1.0f;

    return result;
  }

  /**
   * @brief Calcula a transposta de uma matriz.
   *
   * A transposta de uma matriz é uma nova matriz criada pela troca de linhas por colunas.
   * Ou seja, o elemento na linha `i` e coluna `j` da matriz original se torna o elemento
   * na linha `j` e coluna `i` da matriz transposta.
   *
   * @param mat A matriz original para a qual a transposta deve ser calculada.
   * @return Matrix A matriz transposta de `mat`.
   */
  Matrix MatrixTranspose(Matrix mat)
  {
    Matrix result = {0};

    result.m0 = mat.m0;
    result.m1 = mat.m4;
    result.m2 = mat.m8;
    result.m3 = mat.m12;
    result.m4 = mat.m1;
    result.m5 = mat.m5;
    result.m6 = mat.m9;
    result.m7 = mat.m13;
    result.m8 = mat.m2;
    result.m9 = mat.m6;
    result.m10 = mat.m10;
    result.m11 = mat.m14;
    result.m12 = mat.m3;
    result.m13 = mat.m7;
    result.m14 = mat.m11;
    result.m15 = mat.m15;

    return result;
  }

  /**
   * @brief Convertendo uma matriz 4x4 para um array de floats.
   *
   * @param mat A matriz 4x4 a ser convertida.
   * @return float16 (array de 16 floats)
   */
  float16 MatrixToFloat16(Matrix mat)
  {
    float16 result = {{mat.m0, mat.m1, mat.m2, mat.m3,
                       mat.m4, mat.m5, mat.m6, mat.m7,
                       mat.m8, mat.m9, mat.m10, mat.m11,
                       mat.m12, mat.m13, mat.m14, mat.m15}};

    return result;
  }

  /**
   * @brief Converte um array de floats para uma matriz 4x4.
   *
   * @param f O array de floats (16 indices) a ser convertido.
   * @return Matrix A matriz 4x4 convertida.
   */
  Matrix Flota16ToMatrix(float16 f)
  {
    Matrix result = {0};

    result.m0 = f.v[0];
    result.m1 = f.v[1];
    result.m2 = f.v[2];
    result.m3 = f.v[3];
    result.m4 = f.v[4];
    result.m5 = f.v[5];
    result.m6 = f.v[6];
    result.m7 = f.v[7];
    result.m8 = f.v[8];
    result.m9 = f.v[9];
    result.m10 = f.v[10];
    result.m11 = f.v[11];
    result.m12 = f.v[12];
    result.m13 = f.v[13];
    result.m14 = f.v[14];
    result.m15 = f.v[15];

    return result;
  }

} // namespace core