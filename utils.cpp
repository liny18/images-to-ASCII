#include "utils.hpp"
#include "constants.hpp"

using namespace constants;

// Check if a directory exists
bool directory_exists(const std::string &path)
{
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

// Create a directory
bool create_directory(const std::string &path)
{
    return (mkdir(path.c_str(), 0777) == 0);
}

// Check if a file exists
bool file_exists(const std::string &name)
{
    return (access(name.c_str(), F_OK) != -1);
}

// Remove a file
bool remove_file(const std::string &name)
{
    return (remove(name.c_str()) == 0);
}

// Iterate over files in a directory
void iterate_directory(const std::string &path)
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != nullptr)
    {
        while ((ent = readdir(dir)) != nullptr)
        {
            std::string file_name = ent->d_name;
            // process file_name
        }
        closedir(dir);
    }
}

// need to make sure we're writing to a new file and directory
void check_file_exist()
{
    if (!directory_exists("outputs"))
    {
        create_directory("outputs");
    }

    if (file_exists("outputs/all_output.txt"))
    {
        remove_file("outputs/all_output.txt");
    }
}

// Get the list of image files in a directory
void get_image_files(const std::string &input_directory, std::vector<std::string> &input_files)
{
        DIR *dir = opendir(input_directory.c_str());
    if (dir != nullptr)
    {
        struct dirent *entry;
        while ((entry = readdir(dir)) != nullptr)
        {
            // Check if the entry is a regular file
            if (entry->d_type == DT_REG)
            {
                std::string file_name = entry->d_name;
                // Extract file extension and convert to lower case for comparison
                std::string extension;
                size_t dot_pos = file_name.rfind('.');
                if (dot_pos != std::string::npos && dot_pos + 1 < file_name.length())
                {
                    extension = file_name.substr(dot_pos);
                    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
                }
                // Check if the file is an image based on the extension
                if (extension == ".jpg" || extension == ".jpeg" || extension == ".png")
                {
                    input_files.push_back(input_directory + '/' + file_name);
                }
            }
        }
        closedir(dir);
    }
}

// reverse the characters used for ASCII art. This is used to get the negative of the ASCII image
void reverse_string(std::string &str)
{
    std::reverse(str.begin(), str.end());
}

// display the usage of the program
void show_usage(const std::string &executable_name)
{
    std::cerr << "\nUsage: " << executable_name << " -i <DIRECTORY> [options] \n\n";
    std::cerr << "Options:\n"
                 "  -h, --help              Display this help message\n"
                 "  -i, --input  <DIRECTORY>     Specify the path of the input image directory (required)\n"
                 "  -w, --width  <NUMBER>   Set the width of the ASCII output; maintains aspect ratio\n"
                 "  -s, --chars  <STRING>   Define the set of characters used in the ASCII output\n"
                 "  -p, --print             Print the ASCII output to the console\n"
                 "  -n, --negate            Get the negative of the ASCII image\n"
                 "  -f, --factor            Set the scale factor(a number from 0.0 to 1.0) to resize the image\n"
                 "  -c, --color             Get ASCII PNG's in colors\n\n";

    exit(EXIT_FAILURE);
}

// get the basename of a file, used to match the output file name with the input file name
std::string get_basename(const std::string &full_path)
{
    size_t last_slash = full_path.find_last_of("/\\");
    std::string basename = (last_slash != std::string::npos) ? full_path.substr(last_slash + 1) : full_path;
    size_t last_dot = basename.find_last_of('.');
    if (last_dot != std::string::npos)
    {
        basename = basename.substr(0, last_dot);
    }
    return basename;
}

// parse the command line arguments and set the configuration
void parse_arguments(int argc, char **argv, std::string &input_directory, std::string &executable_name, bool &resize_flag, int &desired_width, bool &print_flag, bool &negate_flag, bool &colored_flag)
{
    struct option long_options[] = {
        {"help", no_argument, nullptr, 'h'},
        {"input", required_argument, nullptr, 'i'},
        {"width", required_argument, nullptr, 'w'},
        {"chars", required_argument, nullptr, 's'},
        {"print", no_argument, nullptr, 'p'},
        {"negate", no_argument, nullptr, 'n'},
        {"factor", required_argument, nullptr, 'f'},
        {"color", no_argument, nullptr, 'c'},
        {0, 0, 0, 0}};

    int option;
    const char *short_options = "hi:w:s:pncf:";
    while ((option = getopt_long(argc, argv, short_options, long_options, nullptr)) != EOF)
    {
        switch (option)
        {
        case 'h':
            show_usage(executable_name);
            break;
        case 'i':
            input_directory = optarg;
            break;
        case 'w':
            desired_width = std::atoi(optarg);
            resize_flag = true;
            break;
        case 's':
            characters = optarg;
            break;
        case 'p':
            print_flag = true;
            break;
        case 'n':
            negate_flag = true;
            break;
        case 'f':
            SCALE_FACTOR = std::atof(optarg);
            break;
        case 'c':
            colored_flag = true;
            break;
        default:
            show_usage(executable_name);
        }
    }
}