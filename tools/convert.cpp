#include <chrono>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <limits>

template<typename VALUE>
void log(std::string msg, VALUE value) {
  std::cout << msg << value << std::endl;
}

int main(int argc, char** argv)
{
  if (argc < 2) {
    std::cout << "Usage: " << *argv << " path/to/your/csv/file" << std::endl;
    return -1;
  }

  // std::cout << "processing..." << std::endl;
  using clock = std::chrono::high_resolution_clock;
  clock::time_point start_time = clock::now();
  start_time = std::chrono::high_resolution_clock::now();

  std::fstream input_csv(argv[1]);

  std::string output_binary_file_name = std::string(argv[1]) + ".bin";
  std::fstream output_binary(output_binary_file_name, std::ios::out | std::ios::binary);

  std::uint32_t min_vertex_id = UINT32_MAX;
  std::uint32_t max_vertex_id = 0;
  std::uint32_t edges = 0;
  std::set<std::uint32_t> record;
  std::string line;

  while(std::getline(input_csv, line)) {
    edges++;
    std::istringstream words(line);
    std::string word;
    while (getline(words, word, ',')) {
      std::uint32_t num = std::stoi(word);
      record.insert(num);
      max_vertex_id = num > max_vertex_id ? num : max_vertex_id;
      min_vertex_id = num < min_vertex_id ? num : min_vertex_id;
      output_binary.write((char*)&num, sizeof(num));
    }
  }

  std::string is_id_continuous = (record.size() == (max_vertex_id - min_vertex_id + 1)) ? "yes" : "no";
  clock::time_point end_time = clock::now();
  long double t_secs = std::chrono::duration_cast<std::chrono::duration<double>>(
                   end_time - start_time).count();

  log("output_file: ", output_binary_file_name);
  log("time: ", std::to_string(t_secs));
  log("edges: ", edges);
  log("vertices: ", record.size());
  log("min_vertex_id: ", min_vertex_id);
  log("max_vertex_id: ", max_vertex_id);
  // log("Are all ids_continuous: ", is_id_continuous);

  input_csv.close();
  output_binary.close();
}

