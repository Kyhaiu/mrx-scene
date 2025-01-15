#include <utils/file.hpp>

namespace utils
{
  /**
   * @brief Carrega um arquivo JSON
   *
   * @param file_path Caminho do arquivo
   * @return json Arquivo JSON
   *
   * @note Retorna um objeto JSON vazio em caso de erro
   */
  json load_json(const std::string &file_path)
  {
    std::ifstream file(file_path);
    json json_file;

    try
    {
      file >> json_file;
    }
    catch (const json::parse_error &e)
    {
      std::cerr << "Erro ao analisar o JSON em '" << file_path << "': " << e.what() << std::endl;
      return json::object(); // Retorna um objeto JSON vazio em caso de erro
    }

    return json_file;
  }

  /**
   * @brief Escreve um arquivo JSON
   *
   * @param file_path Caminho do arquivo
   * @param json_data JSON a ser escrito
   *
   * @return bool Verdadeiro se o arquivo foi escrito com sucesso e falso caso contrário
   */
  void save_json(const std::string &file_path, const json &json_data)
  {
    std::ofstream json_file(file_path);
    if (!json_file.is_open())
    {
      std::cerr << "Erro: Não foi possível abrir o arquivo '" << file_path << "' para escrita." << std::endl;
    }

    json_file << std::setw(4) << json_data << std::endl; // Escreve o JSON formatado com 4 espaços de indentação
    if (json_file.fail())
    {
      std::cerr << "Erro ao escrever no arquivo '" << file_path << "'." << std::endl;
    }
  }
}