#ifndef ARTICLE_REORDER_H 
#define ARTICLE_REORDER_H 

#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <sstream>

#define NUM_OF_ARTICLES 243426
//#include <algorithm>

//#include <iostream>


struct Accumulator {
  int id;
  int start;
  int end;
};

enum ParserState {
  expect_page = 0,
  expect_id,
  expect_pageend
};

int line_count = 0;

int action_get_line_count(std::string s) {
  return line_count;
}

int action_get_id(std::string s) {
  std::string::iterator end_pos = std::remove(s.begin(), s.end(), ' ');
  s.erase(end_pos, s.end());

  std::string tok = "<id>";
  std::string::size_type i = s.find(tok);
  if (i != std::string::npos)
     s.erase(i, tok.length());

  tok = "</id>";
  i = s.find(tok);
  if (i != std::string::npos)
     s.erase(i, tok.length());

//  std::cout << s << std::endl << std::flush;
  return std::stoi(s);
}

void save_pagestart(int res, Accumulator* acc) {
  acc->start = res;
}

void save_id(int res, Accumulator* acc) {
  acc->id = res;
}

void save_pageend(int res, Accumulator* acc) {
  acc->end = res;
}

//bool sortById(Accumulator &A, Accumulator &B) 
//{
//  return (A.id < B.id);
//}

void bubblesort(std::vector<Accumulator>& mylist)
{
	for (int i = 1; i < mylist.size(); i++)
	{

	    for (int j = 0; j < mylist.size() - i; j++) 
	    {
		if (mylist[j].id > mylist[j + 1].id) 
		{
			std::swap(mylist[j], mylist[j + 1]);
//		    Accumulator temp;
//		    temp = mylist[j];
//		    mylist[j] = mylist[j + 1];
//		    mylist[j + 1] = temp;
		}
	    }
	}
}

struct Page {
    std::string content;
    bool written;
};

void reorder() {
    std::vector<Page> pages;
    std::string non_pagedata;
    std::string line;
    bool page_content = false;
    Page page_data;

    // Extract pages
    std::ifstream file(".main");
    if (!file) {
        std::cout << "Unable to open file: " << enwik9_file << std::endl;
        return;
    }

    while (std::getline(file, line)) {
        if (line.find("<page>") != std::string::npos) {
            if (!page_content) {
                page_data = {line, false}; // Start new page_data
            } else {
                pages.push_back(page_data);
                page_data = {line, false}; // Start new page_data
            }
            page_content = true;
        } else {
            if (page_content) {
                page_data.content += line; // Append to current page content
            } else {
                non_pagedata += line; // Accumulate non-page data
            }
        }
    }
    if (page_content) {
        pages.push_back(page_data); // Add last page if exists
    }

    // Write pages to output file
    std::ofstream out_file(".main_reordered");
    if (!out_file) {
        std::cout << "Unable to open output file: " << output_file << std::endl;
        return;
    }

    if (!non_pagedata.empty()) {
        out_file << non_pagedata; // Write non-pagedata first
    }

    std::vector<int> positions;
    std::ifstream order_file_stream(".new_article_order");
    if (!order_file_stream) {
        std::cout << "Unable to open order file: " << order_file << std::endl;
        return;
    }

    std::vector<bool> used(pages.size(), false);
    while (std::getline(order_file_stream, line)) {
        int position = std::stoi(line);
        if (position < pages.size()) {
            if (!used[position]) {
                positions.push_back(position);
                used[position] = true; // Mark as used
            } else {
                std::cout << "Duplicate entry: " << position << std::endl;
            }
        } else {
            std::cout << "Position out of bounds: " << position << std::endl;
        }
    }

    // Fill in unused positions if necessary
    if (positions.size() < pages.size()) {
        for (int i = 0; i < pages.size(); i++) {
            if (!used[i]) {
                positions.push_back(i);
            }
        }
    }

    // Write pages in the specified order
    for (int pos : positions) {
        out_file << pages[pos].content << "\n"; // Write each page content
    }

    out_file.close();
    std::cout << "Finished writing to " << output_file << std::endl;
}

// int reorder() {
//   line_count = 0;

//   std::ifstream file(".main"); //file just has some sentences
//  if (!file) {
//    std::cout << "unable to open file";
//    return -1;
//  }

//   std::ifstream order_file(".new_article_order"); //file just has some sentences
//  if (!order_file) {
//    std::cout << "unable to open file";
//    return -1;
//  }

//   std::vector<std::string> lines;
//   std::vector<int> positions;

//   std::vector<std::string> patterns  = { "<page>", "<id>", "</page>" };
//   std::vector<ParserState>  transitions    = { expect_id, expect_pageend, expect_page };
//   int (*actions[3])(std::string)     = {action_get_line_count, action_get_id, action_get_line_count};
//   void (*save[3])(int, Accumulator*) = {save_pagestart, save_id, save_pageend};

//   ParserState state = expect_page;

//   std::vector<Accumulator> vec;

//   std::string s;
//   std::string pattern;
//   int res = 0;
//   Accumulator acc;
//   while (std::getline(file, s))
//   {
//     pattern = patterns[state];
//     if (s.find(pattern) != std::string::npos) {
//       res = actions[state](s);
//       save[state](res, &acc);
//       state = transitions[state];
//       if (state == expect_page)
//         vec.push_back(acc);
//     } 
//     line_count++;
//     lines.push_back(s);
//   }

//  std::cout << line_count  << std::endl;

//  for(std::vector<Accumulator>::const_iterator it = vec.begin();
//    it != vec.end(); ++it) {
//    std::cout << it->id << " " << it->start << " " << it->end << std::endl;
//  }

//   std::vector<int> used(NUM_OF_ARTICLES, 0);
//   while (std::getline(order_file, s)) {
//    std::cout << s << std::endl << std::flush;
//     positions.push_back(std::stoi(s));
//     std::cout << "used " << used.size() << " Position " << positions.size() << " KMP is full " << std::stoi(s) << std::endl << std::flush;
//     if(std::stoi(s) < used.size())
//       if(used[std::stoi(s)] == 0)
//         used[std::stoi(s)] = 1;
//       else
//         std::cout<< "KMP was really wooden head" << std::endl << std::flush;
//     else
//       std::cout<< "There is error here" << std::endl << std::flush;
//     std::cout << "Passed" << std::endl << std::flush;
//   }
//   std::cout << "finished filling in used" << std::endl << std::flush;
//   if (positions.size() < NUM_OF_ARTICLES) {
//     for (int i = 0; i < NUM_OF_ARTICLES; i++) {
//       if (used[i] == 0) {
//         positions.push_back(i);
//       }
// 	}
//   std::cout << "took alternative filling because smaller size" << std::endl << std::flush;
//   }
//   std::cout << "finished alternative filling in used" << std::endl << std::flush;
			  

//   std::cout << "writing to main_reordered" << std::endl << std::flush;
//   std::ofstream out(".main_reordered");
//   for(int i = 0; i < positions.size(); i++) {
//     std::cout << "positions " << i << " started" << std::endl << std::flush;
//     int pos = positions[i];
//     std::cout << "vec size " << vec.size() << " lines size " << lines.size() << std::endl << std::flush;
//     std::cout << " Id " << vec[pos].id << " Start " << vec[pos].start << " End " << vec[pos].end << std::endl << std::flush;
//     for(int j = vec[pos].start; j <= vec[pos].end; j++) {
//       out << lines[j] << "\n";
//     }
//     std::cout << "positions " << i << " successfully writen" << std::endl << std::flush;
//   } 
//   out.close();
//   std::cout << "finished writing to main_reordered" << std::endl << std::flush;

//   return 0;
// }

int transform() {
  std::ifstream input_file(".main_reordered", std::ios::binary);
    if (!input_file) {
        std::cout << "Unable to open input file" << std::endl;
        return -1;
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
    input_file.close();
    std::cout << "Transformed File read" << std::endl;

    // Create a dictionary of replacements
    std::unordered_map<std::string, std::string> replacements = {
        {"<page>", ""},
        {"</page>", ""},
        {"<title>", ""},
        {"</title>", ""},
        {"<id>", ""},
        {"</id>", ""},
        {"<ip>", ""},
        {"</ip>", ""},
        {"<revision>", ""},
        {"</revision>", ""},
        {"<timestamp>", ""},
        {"</timestamp>", ""},
        {"<contributor>", ""},
        {"</contributor>", ""},
        {"<username>", ""},
        {"</username>", ""},
        {"<comment>", ""},
        {"</comment>", ""}
    };

    // Perform the replacements
    std::string content(buffer.begin(), buffer.end());
    for (const auto& [old, new_str] : replacements) {
        size_t pos = 0;
        while ((pos = content.find(old, pos)) != std::string::npos) {
            content.replace(pos, old.length(), new_str);
            std::cout << "Replaced " << old << " with " << new_str << std::endl;
            pos += new_str.length();
        }
    }

    // Open the binary file for writing
    std::ofstream output_file(".main_reordered", std::ios::binary);
    if (!output_file) {
        std::cout << "Unable to open output file" << std::endl;
        return -1;
    }
    output_file.write(content.c_str(), content.size());
    output_file.close();
    std::cout << "Transformed File written" << std::endl;

    return 0;
}

int sort() {
  line_count = 0;

  std::ifstream file(".main_decomp_restored"); 
  if (!file) {
//    std::cout << "unable to open file";
    return -1;
  }

  std::vector<std::string> lines;

  std::vector<std::string> patterns  = { "<page>", "<id>", "</page>" };
  std::vector<ParserState>  transitions    = { expect_id, expect_pageend, expect_page };
  int (*actions[3])(std::string)     = {action_get_line_count, action_get_id, action_get_line_count};
  void (*save[3])(int, Accumulator*) = {save_pagestart, save_id, save_pageend};

  ParserState state = expect_page;

  std::vector<Accumulator> vec;

  std::string s;
  std::string pattern;
  int res = 0;
  Accumulator acc;
  while (std::getline(file, s))
  {
    pattern = patterns[state];
    if (s.find(pattern) != std::string::npos) {
      res = actions[state](s);
      save[state](res, &acc);
      state = transitions[state];
      if (state == expect_page)
        vec.push_back(acc);
    } 
    line_count++;
    lines.push_back(s);
  }

//  std::cout << line_count  << std::endl;

//  std::sort(vec.begin(), vec.end(), sortById);

  bubblesort(vec);

//  for(std::vector<Accumulator>::const_iterator it = vec.begin();
//    it != vec.end(); ++it) {
//    std::cout << it->id << " " << it->start << " " << it->end << std::endl;
//  }

  std::ofstream out(".main_decomp_restored_sorted");
  if (!out) {
//    std::cout << "unable to open file";
    return -1;
  }
  for(int i =0; i < vec.size(); i++) {
    for(int j = vec[i].start; j <= vec[i].end; j++) {
      out << lines[j] << "\n";
    }
  } 
  out.close();

  return 0;
}

#endif // ARTICLE_REORDER_H 
