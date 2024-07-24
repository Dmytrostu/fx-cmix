import xml.etree.ElementTree as ET
import re

def extract_pages(enwik9_file):
    pages = []
    non_pagedata = b''
    page_data = {}

    page_content = False
    with open(enwik9_file, 'rb') as file:
        for line in file:
            if line.strip().startswith(b"<page>"):
                if not page_content:
                    page_data = {'content': line, 'written' : False} # only start with page_data

                else:
                    pages.append(page_data)
                    page_data = {'content': line, 'written' : False} # only start with page_data

                page_content = True

            else:
                if page_content:
                    page_data['content'] = page_data['content'] + line
                    
                else:
                    non_pagedata = non_pagedata + line
        
        if page_data:
            pages.append(page_data)



    return pages, non_pagedata            


def write_pages_to_file(pages,non_pagedata, order_file, output_file):
    with open(output_file, 'wb') as out_file:
        if non_pagedata:
            out_file.write(non_pagedata)

        cnt = 0

        with open(order_file, 'r', encoding = 'utf-8') as file:
            for line in file:
                # if cnt > len(pages):
                    # print(cnt)
                    # return
                # cnt = cnt + 1
                striped_line = line.strip()
                if int(striped_line) < len(pages):
                    out_file.write(pages[int(striped_line)]['content'])
                    pages[int(striped_line)]['written'] = True
                else:
                    print('does not exist:', striped_line)
            
            for page in pages:
                if page['written'] == False:
                    out_file.write(page['content'])
                    pass



if __name__ == "__main__":
    enwik9_file = 'enwik9'
    output_file = 'modified_enwik9'
    order_file = 'new_article_order'

    # Extract pages
    pages, non_pagedata = extract_pages(enwik9_file)
    print('total : ', len(pages), ' articles')


    # Reverse the order of pages and write them to the output file
    write_pages_to_file(pages, non_pagedata, order_file, output_file)
