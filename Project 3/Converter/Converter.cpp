///////////////////////////////////////////////////////////////////////////
// Converter.cpp : implements and provides test stub for Converter.h     //
// ver 1.0                                                               //
//                                                                       // 
// Application   : OOD-S19 Instructor Solution                           //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        :	Ammar,  Syracuse University                          //
//					Yihang Nan, Syracuse University                      //
//                 315-863-0892, ynan02@syr.edu                          //
///////////////////////////////////////////////////////////////////////////

#include "Converter.h"
#include "../Logger/Logger.h"
#include "../FileSystem/FileSystem.h"

using namespace FileSystem;
using namespace Logging;

// -----< default ctor >--------------------------------------------------
CodeConverter::CodeConverter()
{
}
//------------------<get the rows number of class>---------------------
void CodeConverter::row_class(std::map<int, int> temp_class)
{
	row = temp_class;
}
//-----------------<get the rows number of function>--------------------
void CodeConverter::row_function(std::map<int, int> temp_func)
{
	func = temp_func;
}
// -----< ctor to set dep table >-----------------------------------------
CodeConverter::CodeConverter(const DependencyTable & dt) : dt_(dt)
{
  LoggerDebug::write("\n  CodeConverter initialized with DependencyTable\n");
}

// -----< set dependency table function >---------------------------------
void CodeConverter::setDepTable(const DependencyTable & dt)
{
  dt_ = dt;
}

// -----< convert pre-set dependency table >------------------------------
std::vector<std::string> CodeConverter::convert()
{
  if (!createOutpurDir()) {
    LoggerDebug::write("\n\n  Failed to start conversion due to inability to create output directory");
    LoggerDebug::write("\n  -- Output Directory: " + Path::getFullFileSpec(outputDir_));
    return std::vector<std::string>();
  }

  LoggerDemo::write("\n\n  Converting files to webpages");
  LoggerDebug::write("\n\n  Converting files to webpages");

  LoggerDebug::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));
  LoggerDemo::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));

  for (auto entry : dt_) {
    if (convertFile(entry.first)) {
      LoggerDemo::write("\n  -- Converted: ");
      LoggerDebug::write("\n  -- Converted: ");
    }
    else {
      LoggerDemo::write("\n  -- Failed:    ");
      LoggerDebug::write("\n  -- Failed:    ");
    }

    std::string filename = Path::getName(entry.first);
    LoggerDebug::write(filename);
    LoggerDemo::write(filename);
  }
  LoggerDebug::write("\n");
  LoggerDemo::write("\n");
  return convertedFiles_;
}

// -----< convert single file given path >----------------------------------
std::string CodeConverter::convert(const std::string & filepath)
{
  if (!createOutpurDir()) {
    LoggerDebug::write("\n\n  Failed to start conversion due to inability to create output directory");
    LoggerDebug::write("\n  -- Output Directory: " + Path::getFullFileSpec(outputDir_));
    return "";
  }

  LoggerDemo::write("\n\n  Converting files to webpages");
  LoggerDebug::write("\n\n  Converting files to webpages");

  LoggerDebug::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));
  LoggerDemo::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));

  if (convertFile(filepath)) {
    LoggerDemo::write("\n  -- Converted: ");
    LoggerDebug::write("\n  -- Converted: ");
  }
  else {
    LoggerDemo::write("\n  -- Failed:    ");
    LoggerDebug::write("\n  -- Failed:    ");
  }

  std::string filename = Path::getName(filepath);
  LoggerDebug::write(filename);
  LoggerDemo::write(filename);

  return filename + ".html";
}
//---------------<convert and display>-----------------------------------------
std::vector<std::string> CodeConverter::convert(const std::vector<std::string>& files)
{
  clear();
  dt_ = DependencyTable(files);
  return convert();
}

// -----< get set output directory >----------------------------------------
const std::string & CodeConverter::outputDir() const
{
  return outputDir_;
}

// -----< set output directory >--------------------------------------------
void CodeConverter::outputDir(const std::string & dir)
{
  outputDir_ = dir;
}

// -----< create output directory >-----------------------------------------
/* returns success of creation. it could fail if dir is sys protected */
bool CodeConverter::createOutpurDir()
{
  if (!Directory::exists(outputDir_)) {
    LoggerDebug::write("\n  Creating output directory. Path: " + Path::getFullFileSpec(outputDir_));
    return Directory::create(outputDir_);
  }
  LoggerDebug::write("\n  Output directory already exists. Proceeding...");
  return true;
}

// -----< get list of converted files >-------------------------------------
const std::vector<std::string> CodeConverter::convertedFiles() const
{
  return convertedFiles_;
}
//-------------<clear the converter files_>---------------------
void CodeConverter::clear()
{
  convertedFiles_.clear();
  dt_.clear();
}

// -----< private - read file and create webpage >--------------------------
bool CodeConverter::convertFile(std::string file)
{
  std::ifstream in(file);
  if (!in.is_open() || !in.good()) {
    LoggerDebug::write("Error -- unable to read file, path may be invalid.");
    in.close();
    return false;
  }

  std::string filename = Path::getName(file);
  std::string outputPath = outputDir_ + filename + ".html";
  out_.open(outputPath, std::ofstream::out);
  if (!out_.is_open() || !out_.good()) {
    LoggerDebug::write("Error -- unable to open output file for writing.");
    in.close();
    return false;
  }
  
  // write everything to file
  //PrintDenpen();
  addPreCodeHTML(filename);
  addDependencyLinks(file);
  addbutton();
  addPreTag();

  while (in.good()) {
    std::string line;
    while (std::getline(in, line)) {
		count_rows++;
		docheck_class();
		docheck_func();
		//out_ << line << std::endl;
		skipSpecialChars(line);
		docheck_com(line);
    }
  }

  addClosingTags();
  out_.close();

  convertedFiles_.push_back(outputPath);
  return true;
}
//------------<span the class>---------------------
void CodeConverter::docheck_class()
{
	if (row.count(count_rows) == 1)
	{
		out_ << "<span class=\"toggleClassDisplay\">" << std::endl;
		mark_row = row[count_rows]+1;
	}
	if (count_rows == mark_row)
	{
		out_ << "</span>" << std::endl;
	}
	//return mark_row;
}
//-----------<span the function>-----------------
void CodeConverter::docheck_func()
{
	if (func.count(count_rows) == 1)
	{
		out_ << "<span class=\"toggleMethodDisplay\">{" << std::endl;
		mark_func = func[count_rows]+1;
	}
	if (count_rows == mark_func)
	{
		out_ << "</span>" << std::endl;
	}
	//return mark_func;
}
//------------<get the table for converter>---------------
void CodeConverter::getTable_converter(std::unordered_map<std::string, std::vector<std::string>> temp)
{
	table_read = temp;
}
//------------<get the file name for converter>---------------
void CodeConverter::get_file_name(std::string pwd_f)
{
	pwd_file = pwd_f;
}
//------------<print Dependency>-------------------------
void CodeConverter::PrintDenpen()
{
	std::vector<std::string> out_print_table;
	for (int i = 0; i < table_read[pwd_file].size(); i++)
	{
		std::string temp = table_read[pwd_file][i];
		if (temp.find_last_of("\/") != temp.npos)
		{
			int pos = temp.find_last_of("\/");
			int len = temp.size() - pos;
			temp = temp.substr(pos+1, len) + ".html";
			out_print_table.push_back(temp);
		}
		else {
			temp = temp + ".html";
			out_print_table.push_back(temp);
		}
	}
	for (auto i : out_print_table)
	{
		out_ << "<a href = \"" << i << "\">" << i << "</a>" << std::endl;
	}
}
//------------<hide the comments in the code>--------------------
void CodeConverter::docheck_com(std::string line)
{
	if (line.size() >= 2)
	{
		if (line.find("\//") != std::string::npos)// || line.find("")!= std::string::npos) //need to fix

		{
			out_ << "<span class=\"toggleCommentsDisplay\">" << std::endl;
			out_ << line << std::endl;
			out_ << "</span>" << std::endl;
		}
		else if (line.find("\/*") != std::string::npos)
		{
			out_ << "<span class=\"toggleCommentsDisplay\">" << std::endl;
			out_ << line << std::endl;
		}
		else if (line.find("*\/") != std::string::npos)
		{
			out_ << line << std::endl;
			out_ << "</span>" << std::endl;
		}
		else
		{
			out_ << line << std::endl;
		}
	}
	else {
		out_ << line << std::endl;
	}
}


//----------<add button on the page> --------------------------
void CodeConverter::addbutton()
{
	out_ << "<div>" << std::endl;
	out_ << "<button onclick=\"showHideClass('toggleClassDisplay')\">Show/Hide Class Contents" << std::endl;
	out_ << "</button>" << std::endl;
	out_ << "<button onclick=\"showHideClass('toggleMethodDisplay')\">Show/Hide Method Contents" << std::endl;
	out_ << "</button>" << std::endl;
	out_ << "<button onclick = \"showHideClass('toggleCommentsDisplay')\">Show/Hide Comments Contenst"<< std::endl;
	out_ << "</button>" << std::endl;
	out_ << "</div>" << std::endl;
}
// -----< private - add generic HTML preliminary markup >-------------------
void CodeConverter::addPreCodeHTML(const std::string& title)
{
  out_ << "<DOCTYPE !HTML>" << std::endl;
  out_ << "<html>" << std::endl;
  out_ << "  <head>" << std::endl;
  out_ << "    <Title>" << title << "</Title>" << std::endl;
  out_ << "    <style>" << std::endl;
  out_ << "      body {" << std::endl;
  out_ << "        padding:15px 40px;" << std::endl;
  out_ << "        font-family: Consolas;" << std::endl;
  out_ << "        font-size: 1.25em;" << std::endl;
  out_ << "        font-weight: normal;" << std::endl;
  out_ << "      }" << std::endl;
  out_ << "			span {" << std::endl;
  out_ << "			        display:inline;" << std::endl;
  out_ << "				  }" << std::endl;
  out_ << "			div{" << std::endl;
  out_ << "				position:fixed;" << std::endl;
  out_ << "				top:10px;" << std::endl;
  out_ << "				right:10px;" << std::endl;
  out_ << "				}" << std::endl;
  out_ << "				.toggleClassDisplay {}" << std::endl;
  out_ << "				.toggleMethodDisplay {}" << std::endl;
  out_ << "				.toggleCommentsDisplay{}" << std::endl;
  out_ << "      </style>" << std::endl;
  out_ << "	 <script>" << std::endl;
  out_ << "	function showHideClass(className) {" << std::endl;
  out_ << "	let elems = document.getElementsByClassName(className);" << std::endl;
  out_ << "	if (elems === null) {" << std::endl;
  out_ << "		return;}" << std::endl;
  out_ << "	 for (let i = 0; i < elems.length; ++i) {" << std::endl;
  out_ << "		let displayProp = elems[i].style.display;" << std::endl;
  out_ << "		if (displayProp === 'none')" << std::endl;
  out_ << "			elems[i].style['display'] = \"inline\";" << std::endl;
  out_ << "		else" << std::endl;
  out_ << "		elems[i].style['display'] = \"none\";}}" << std::endl;
  out_ << "  </script" << std::endl;
  out_ << "  </head>" << std::endl << std::endl;
  out_ << "  <body>" << std::endl;
}

// -----< private - add pre tag >------------------------------------------
/* seperated into seperate function to allow for dependencies addition
*  before the actual code of the file */
void CodeConverter::addPreTag()
{
  out_ << "    <pre>" << std::endl;
}

// -----< private - add depedency links markup code >----------------------
void CodeConverter::addDependencyLinks(std::string file)
{
  std::string filename = Path::getName(file);
  if (!dt_.has(file)) { // in case of single file conversion
    LoggerDebug::write("\n    No entry found in DependencyTable for [" + filename + "]. Skipping dependency links..");
    return;
  }

  if (dt_[file].size() == 0) { // in case the file has no dependencies
    LoggerDebug::write("\n    No dependencies found for [" + filename + "]. Skipping dependency links..");
    return;
  }

  out_ << "    <h3>Dependencies: " << std::endl;
  for (auto dep : dt_[file]) {
    out_ << "      <a href=\"" << Path::getName(dep) << ".html\">" << Path::getName(dep) << "</a>" << std::endl;
  }
  out_ << "    </h3>";
}

// -----< private - add generic HTML markup closing tags >-----------------
void CodeConverter::addClosingTags()
{
  out_ << "    </pre>" << std::endl;
  out_ << "  </body>" << std::endl;
  out_ << "</html>" << std::endl;
}

// -----< private - replace HTML special chars >---------------------------
/* note: the function uses while loop to make sure ALL special characters
*  are replaced instead of just the first encounter. */
void CodeConverter::skipSpecialChars(std::string & line)
{
  size_t pos = line.npos;
  while((pos = line.find('<')) != line.npos) 
    line.replace(pos, 1, "&lt;");

  while ((pos = line.find('>')) != line.npos)
    line.replace(pos, 1, "&gt;");
}


#ifdef TEST_CONVERTER

// -----< test stub for converter class >-----------------------------------
int main() {
  LoggerDebug::attach(&std::cout);
  LoggerDebug::start();

  LoggerDebug::title("Testing CodeConverter functions");

  LoggerDebug::stop(); // do not log DependencyTable logs
  
  DependencyTable dt;
  dt.addDependency("Converter.h", "Converter.cpp");
  dt.addDependency("Converter.cpp", "Converter.h");
  dt.addDependency("A.h", "A.h");
  
  LoggerDebug::start(); // log from now on

  CodeConverter cc(dt);
  dt.display();
  LoggerDebug::write("\n");
  //cc.outputDir("C:\\windows\\system32\\"); // will trigger errors which are handled
  cc.convert();

  LoggerDebug::stop();
  return 0;
}

#endif