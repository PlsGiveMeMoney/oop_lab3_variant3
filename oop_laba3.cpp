#include <iostream>
#include <fstream>
#include <string>

struct scan_info
{
    std::string model; // наименование модели
    int price; // цена
    double x_size; // горизонтальный размер области сканирования
    double y_size; // вертикальный размер области сканирования
    int optr; // оптическое разрешение
    int grey; // число градаций серого
};

void insert(scan_info& info)
{
    std::ifstream fin("scan_info.txt", std::ios::in);
    if (fin.is_open())
    {
        fin.seekg(0, std::ios::end);
        int bytes = fin.tellg();
        int count = 0;
        fin.seekg(0, std::ios::beg);
        fin.read((char*)&count, sizeof(int));
        char* readBytes = new char[bytes - 4];
        fin.read(readBytes, bytes - 4);
        fin.close();

        std::ofstream fout("scan_info.txt", std::ios::out | std::ios::binary);
        count++;
        fout.write((char*)&count, sizeof(int));
        fout.write(readBytes, bytes - 4);
        fout.write((char*)&info.price, sizeof(int));
        fout.write((char*)&info.x_size, sizeof(double));
        fout.write((char*)&info.y_size, sizeof(double));
        fout.write((char*)&info.optr, sizeof(int));
        fout.write((char*)&info.grey, sizeof(int));
        fout.write(info.model.c_str(), info.model.size() + 1);
        fout.close();
        delete[] readBytes;
    }
    else
    {
        std::ofstream fout("scan_info.txt", std::ios::out | std::ios::binary);
        int count = 1;
        fout.write((char*)&count, sizeof(int));
        fout.write((char*)&info.price, sizeof(int));
        fout.write((char*)&info.x_size, sizeof(double));
        fout.write((char*)&info.y_size, sizeof(double));
        fout.write((char*)&info.optr, sizeof(int));
        fout.write((char*)&info.grey, sizeof(int));
        fout.write(info.model.c_str(), info.model.size() + 1);
        fout.close();
    }
}

void get(scan_info*& info, int& n)
{
    std::ifstream fin("scan_info.txt", std::ios::in | std::ios::binary);

    if (fin.is_open())
    {
        fin.read((char*)&n, sizeof(int));
        info = new scan_info[n];

        char* readByte = new char;
        for (int i = 0; i < n; i++)
        {
            fin.read((char*)&info[i].price, sizeof(int));
            fin.read((char*)&info[i].x_size, sizeof(double));
            fin.read((char*)&info[i].y_size, sizeof(double));
            fin.read((char*)&info[i].optr, sizeof(int));
            fin.read((char*)&info[i].grey, sizeof(int));

            *readByte = 1;
            while (*readByte != 0)
            {
                fin.read(readByte, 1);
                if (*readByte != 0)
                    info[i].model += *readByte;
            }
        }
        delete readByte;
    }
    else
    {
        info = nullptr;
        n = 0;
    }
    fin.close();
}

void insertDialog()
{
    scan_info scan;
    std::cout << "Enter Model: ";
    std::cin.ignore(INT_MAX, '\n');
    std::getline(std::cin, scan.model);
    std::cout << "Enter Info (price, x_size, y_size, optr, grey): ";
    if ((std::cin >> scan.price >> scan.x_size >> scan.y_size >> scan.optr >> scan.grey))
        insert(scan);
    else
        std::cout << "Incorrect input!" << std::endl;
}

void showDialog()
{
    int n;
    scan_info* s;
    get(s, n);

    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = i; j < n; j++)
        {
            if (s[i].model > s[j].model)
            {
                scan_info tmp = s[j];
                s[j] = s[i];
                s[i] = tmp;
            }
        }
    }

    for (size_t i = 0; i < n; i++)
    {
        std::cout << s[i].model << " " << s[i].price << " " << s[i].x_size << " " << s[i].y_size << " " << s[i].optr << " " << s[i].grey << " " << std::endl;
    }
    delete[] s;
}


int main()
{
    std::cout << "1. Enter scan info\n2. Show scans\n\nDialog: ";
    int d;
    if ((std::cin >> d))
    {
        if (d == 1)
        {
            std::cout << std::endl;
            insertDialog();
        }
        else if (d == 2)
        {
            std::cout << std::endl;
            showDialog();
        }
        else
            std::cout << "Unknown dialog!" << std::endl;
    }
    else
        std::cout << "Incorrect input!" << std::endl;;

    return 0;
}
