#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "boost/program_options.hpp"
#include "boost/filesystem.hpp"

int parseInputArguments(int argc, char *argv[],
                        std::string & InputPath)
{
  boost::program_options::options_description description("InputPath");

  description.add_options()
    ("help", "produce help message")
    ("InputPath", boost::program_options::value< std::string >(), "Directory that contains all data images (required)")
  ;

  boost::program_options::variables_map variableMap;
  boost::program_options::store(
    boost::program_options::parse_command_line(argc, argv, description), 
    variableMap
  );
  boost::program_options::notify(variableMap);

  if ( variableMap.count("help") || argc == 1 ) {
    std::cout << description << std::endl;
    return EXIT_FAILURE;
  }

  if ( variableMap.count("InputPath") ) {
    InputPath = variableMap["InputPath"].as< std::string >();
    std::cout << "Input directory: " << InputPath << std::endl;
  } else {
    std::cerr << "Directory not provided" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}


int main(int argc, char *argv[])
{

  std::string InputPathName;

  if(parseInputArguments(argc, argv, InputPathName))
    return EXIT_FAILURE;

  boost::filesystem::path InputPath(InputPathName);
  std::vector<boost::filesystem::path> ImagePaths; 

  if ( !boost::filesystem::exists( InputPath ) ) { return EXIT_FAILURE; }
  boost::filesystem::directory_iterator end_itr;

  for ( boost::filesystem::directory_iterator itr( InputPath );
        itr != end_itr;
        ++itr )
  {
    if (boost::filesystem::extension(itr->path()) == ".jpg" ||
        boost::filesystem::extension(itr->path()) == ".JPG")
    {
      ImagePaths.push_back(itr->path());
    }
  }

  std::cout << "Processing " << ImagePaths.size() << " images." << std::endl;
  cv::Mat SizeImage = cv::imread(ImagePaths[0].c_str(), CV_LOAD_IMAGE_GRAYSCALE);

  cv::Mat ImageWeigths  = cv::Mat::zeros(SizeImage.size(), CV_32FC1);
  cv::Mat ImageIndexs   = cv::Mat::zeros(SizeImage.size(), CV_32FC1);

  cv::Mat FinalImage(ImageIndexs.size(), CV_8UC3);

  for(size_t i = 0; i < ImagePaths.size(); ++i) {
    cv::Mat img = cv::imread(ImagePaths[i].c_str(), CV_LOAD_IMAGE_COLOR);
    
    for(int row = 0; row < img.rows; ++row) {
      for(int col = 0; col < img.cols; ++col) {
        cv::Vec3f RGBPixel = static_cast<cv::Vec3f>( img.at<cv::Vec3b>(row, col) );

        float weight = 1. / (RGBPixel[0]+RGBPixel[1]+RGBPixel[2]);

        if(weight > ImageWeigths.at<float>(row, col)) {
          ImageWeigths.at<float>(row, col)  = weight;
          ImageIndexs.at<float>(row, col)   = i;

          FinalImage.at<cv::Vec3b>(row, col) = img.at<cv::Vec3b>(row, col);
        }
      }
    }
  }

  cv::imwrite("final.png", FinalImage);

  return EXIT_SUCCESS;
}