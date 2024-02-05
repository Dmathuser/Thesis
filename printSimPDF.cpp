
#include <simulation.h>
#include <fstream>
#include <cstring>
#include <cerrno>
#include <cstdlib>

using namespace std;

#define CELLSIZE 150

static void figheader(ofstream& fout)
{
  fout << "#FIG 3.2  Produced by xfig version 3.2.9\n";
  fout << "#encoding: UTF-8\n";
  fout << "Landscape\n";
  fout << "Center\n";
  fout << "Inches\n";
  fout << "Letter\n";
  fout << "100.00\n";
  fout << "Single\n";
  fout << "-2\n";
  fout << "1200 2\n";
}

/*  The line() function emits a fig polyline.  From the Fig Format manual:
    (3.5) POLYLINE
    ==============

    First line:
    type	name			(brief description)
    ----	----			-------------------
    int	object_code		(always 2)
    int	sub_type		(1: polyline
    2: box
    3: polygon
    4: arc-box)
    5: imported-picture bounding-box)
    int	line_style		(enumeration type, solid, dash, dotted, etc.)
    int	thickness		(1/80 inch)
    int	pen_color		(enumeration type, pen color)
    int	fill_color		(enumeration type, fill color)
    int	depth			(enumeration type)
    int	pen_style		(pen style, not used)
    int	area_fill		(enumeration type, -1 = no fill)
    float	style_val		(1/80 inch, specification for dash/dotted lines)
    int	join_style		(enumeration type)
    int	cap_style		(enumeration type, only used for POLYLINE)
    int	radius			(1/80 inch, radius of arc-boxes)
    int	forward_arrow		(0: off, 1: on)
    int	backward_arrow		(0: off, 1: on)
    int	npoints			(number of points in line)

    Points line(s).  The x,y coordinates follow, any number to a line, with
    as many lines as are necessary:

    type	name			(brief description)
    ----	----			-------------------
    int	x1, y1			(Fig units)
    int	x2, y2			(Fig units)
    .
    .
    int	xnpoints ynpoints	(this will be the same as the 1st
    point for polygon and box)
*/


static void line(ofstream& fout,int x1,int y1,int x2,int y2)
{
 
  fout << "2 1 0 1 0 7 50 -1 -1 0.000 0 0 -1 0 0 2\n";
  fout << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << endl;
}

bool Simulation::printSimPDF(string outFileName)
{
  string figFileName;
  ofstream fout;
  cout << figFileName << " to " << outFileName << endl;

  // Make sure filenames end in .pdf and .fig
  if((outFileName.find_last_of('.') != string::npos) &&
     (outFileName.substr(outFileName.find_last_of('.'),4) == ".pdf"))
    figFileName=outFileName.substr(0,outFileName.find_last_of('.'))+"fig";
  else
    {
      figFileName = outFileName+".fig";
      outFileName += ".pdf";
    }

  cout << figFileName << " to " << outFileName << endl;
  
// otherwise, create the fig file name by chopping of pdf and adding fig
  
  fout.open(figFileName);
  if(!fout)
    {
      cerr << "printSimPDF() failed to open file '"<<figFileName<<
	"' for output: " << strerror(errno) << '\n';
      return 1;
    }
    
  figheader(fout);
  
  for (int j = 0; j < height; j++)
    {
      for (int i = 0; i < width; i++)
	{
	  // horizontal line on top
	  if(states[j][i].up == '-')
	    line(fout,i*CELLSIZE,j*CELLSIZE,(i+1)*CELLSIZE,j*CELLSIZE);

	  // horizontal line on bottom
	  if(states[j][i].down == '-')
	    line(fout,i*CELLSIZE,(j+1)*CELLSIZE,(i+1)*CELLSIZE,(j+1)*CELLSIZE);
	  
	  // vertical line on left
	  if(states[j][i].left == '|')
	    line(fout,i*CELLSIZE,j*CELLSIZE,i*CELLSIZE,(j+1)*CELLSIZE);

	  // vertical line on right
	  if(states[j][i].right == '|')
	    line(fout,(i+1)*CELLSIZE,j*CELLSIZE,(i+1)*CELLSIZE,(j+1)*CELLSIZE);
	      
	}
      std::cout << std::endl;
    }
  
  fout.close();

  string convert_command = "fig2dev -L pdf "+figFileName+' '+outFileName;
  
  if(system(convert_command.c_str()) == -1)
        {
	  cerr << "printSimPDF() failed run 'fig2dev "<<figFileName<<"'  '" <<
	    outFileName << "' : " << strerror(errno) << '\n';
      return 1;
    }

  
  return true;
}

