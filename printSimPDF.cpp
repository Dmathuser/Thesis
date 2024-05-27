/* To do:
   There are many duplicate lines, and it can be drawn with long lines
   instead of many short lines. Instead of immediately outputing the
   lines, add them to a list.
   1) If there is a duplicate to the new line, ignore the new line.
   2) If there is another line that begins or ends at either endpoint
   of the new line, then add a point to the existing line, and remove
   the previous endpoint of that line if possible.
   Output the final list.
   
*/
   
   
#include <simulation.h>
#include <fstream>
#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <cmath>

using namespace std;

#define CELLSIZE 600

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

static void line(ofstream& fout,int x1,int y1,int x2,int y2)
{
  fout << "2 1 0 1 0 7 50 -1 -1 0.000 0 0 -1 0 0 2\n";
  fout << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << endl;
}

static void arrow(ofstream& fout,int x1,int y1,int x2,int y2)
{
  cout << "creating arrow\n";
  fout << "2 1 0 1 0 7 50 -1 -1 0.000 0 0 -1 1 0 2\n";
  fout << "1 1 1.00 60.00 120.00\n";
  fout << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << endl;
}

// Given three points, find the center of the circle that passes
// through them.  If the points are colinear, then no solution exists,
// so return false. Note: this is also a test for colinearity between
// three points.  To understand this, you may have to get out pencil
// and paper and remember some basic geometry.
static bool circleCenter(double x1, double y1,   // p1
			 double x2, double y2,   // p2
			 double x3, double y3,   // p3
			 double &cx, double &cy)
{
  // ux and uy define a vector orthogonal to the line between p1 and p2
  double ax = (x1 + x2) / 2.0;  // x midpoint between p1 and p2
  double ay = (y1 + y2) / 2.0;  // y midpoint between p1 and p2
  double ux = (y1 - y2);        // directed distance from p1 to p2 in y direction
  double uy = (x2 - x1);        // directed distance from p1 to p2 in x direction
  // vx and vy define a vector orthogonal to the line between p2 and p3
  double bx = (x2 + x3) / 2.0;  // x midpoint between p2 and p3
  double by = (y2 + y3) / 2.0;  // y midpoint between p2 and p3
  double vx = (y2 - y3);        // directed distance from p2 to p3 in x direction
  double vy = (x3 - x2);        // directed distance from p2 to p3 in y direction
  // now we find the intersection of two orthogonal vectors from the midpoints
  double dx = ax - bx;          // directed distance between midpoints
                                // of (p1,p2) and (p2,p3) in x direction.
  double dy = ay - by;          // directed distance between midpoints
                                // of (p1,p2) and (p2,p3) in y direction.
  double vu = vx * uy - vy * ux;// magic of geometry.
  if (vu == 0.0)
    return false; // Points are collinear, so no unique solution
  
  double g = (dx * uy - dy * ux) / vu;  // more magic
  // if vu is very small, then we could end with g being +/- inf.  In
  // that case, we consider the three points to be colinear and keep
  // going.
  if(isinf(g))
    return false;
  cx = bx + g * vx;
  cy = by + g * vy;
  // also check for final results nan or Inf.  If we get a NaN or
  // Inf. then it is time to die.
  if(isnan(g) || isnan(cx) || isnan(cy) || isinf(cx) || isinf(cy))
    {
      cerr << "circleCenter got a NaN or Inf\n";
      // If this ever happens, you can add a report on what the inputs
      // were, or set a breakpoint in the debugger.
      exit(1);
    }
  return true;
}

// Emit an arc from p1 to p3, passing through p2.  The order of the
// points matters!  The arc is drawn in a clockwise direction. If you
// swap p1 and p3, then you get the part of the circle that you do
// not want, rather than the part that you do want.
static void arc(ofstream& fout,int x1,int y1,int x2,int y2,int x3,int y3)
{
  double cx,cy;
  if(!circleCenter(x1,y1,x2,y2,x3,y3,cx,cy))
    line(fout,x1,y1,y3,y3); // if the points are colinear, just draw a line
  else
    {
      fout << "5 1 0 1 0 0 50 -1 10 0.000 0 1 0 0 " << 
	cx << ' ' << cy << ' ' <<
	x1 << ' ' << y1 << ' ' <<
	x2 << ' ' << y2 << ' ' <<
	x3 << ' ' << y3 << endl;
    }
}

// Create a PDF image of the gridworld by creating a Fig file then
// using fig2dev from the transfig package.  Fig2dev can create many
// other formats as well.
bool Simulation::printSimPDF(string outFileName)
{
  string figFileName;
  ofstream fout;
  // Make sure filenames end in .pdf and .fig
  if((outFileName.find_last_of('.') != string::npos) &&
     (outFileName.substr(outFileName.find_last_of('.'),4) == ".pdf"))
    figFileName=outFileName.substr(0,outFileName.find_last_of('.'))+"fig";
  else
    {
      figFileName = outFileName+".fig";
      outFileName += ".pdf";
    }
  // Open the fig file
  fout.open(figFileName);
  if(!fout)
    {
      cerr << "printSimPDF() failed to open file '"<<figFileName<<
	"' for output: " << strerror(errno) << '\n';
      return false;
    }
  // Write the fig file header
  figheader(fout);
  // Draw stuff
  for (int j = 0; j < height; j++)
    {
      for (int i = 0; i < width; i++)
	{
	  // horizontal line on top
	  if(states[j][i].up == '-')
	    {
	      line(fout,i*CELLSIZE,j*CELLSIZE,(i+1)*CELLSIZE,j*CELLSIZE);
	      // if not topmost line in image, could be one-way wall
	      if((j>0) &&
		 (states[j-1][i].down != '-') &&
		 (states[j-1][i].down != 'v'))
		arrow(fout,
		      i*CELLSIZE+CELLSIZE/2,j*CELLSIZE-CELLSIZE/3,
		      i*CELLSIZE+CELLSIZE/2,j*CELLSIZE+CELLSIZE/3);
	    }
	  // horizontal line on bottom
	  if(states[j][i].down == '-')
	    // if not bottommost line in image, could be one-way wall
	    {
	      line(fout,i*CELLSIZE,(j+1)*CELLSIZE,(i+1)*CELLSIZE,(j+1)*CELLSIZE);
	      if((j<(height-1)) &&
		 (states[j+1][i].up != '-') &&
		 (states[j+1][i].up != '^'))
		arrow(fout,
		      i*CELLSIZE+CELLSIZE/2,(j+1)*CELLSIZE+CELLSIZE/3,
		      i*CELLSIZE+CELLSIZE/2,(j+1)*CELLSIZE-CELLSIZE/3);
	    }
	  // vertical line on left
	  if(states[j][i].left == '|')
	    {
	      line(fout,i*CELLSIZE,j*CELLSIZE,i*CELLSIZE,(j+1)*CELLSIZE);
	      // if not leftmost column in image, could be one-way wall
	      if((i>0) &&
		 (states[j][i-1].right != '-') &&
		 (states[j][i-1].right != '>'))
		arrow(fout,
		      i*CELLSIZE-CELLSIZE/3,j*CELLSIZE+CELLSIZE/2,
		      i*CELLSIZE+CELLSIZE/3,j*CELLSIZE+CELLSIZE/2);
	    }
	  // vertical line on right
	  if(states[j][i].right == '|')
	    {
	      line(fout,(i+1)*CELLSIZE,j*CELLSIZE,(i+1)*CELLSIZE,(j+1)*CELLSIZE);
	      // if not rightmost column in image, could be one-way wall
	      if((i<(width-1)) &&
		 (states[j][i+1].left != '-') &&
		 (states[j][i+1].left != '<'))
		arrow(fout,
		      (i+1)*CELLSIZE+CELLSIZE/3,j*CELLSIZE+CELLSIZE/2,
		      (i+1)*CELLSIZE-CELLSIZE/3,j*CELLSIZE+CELLSIZE/2);
	    }
	  // horizontal arch on top
	  if(states[j][i].up == '^')
	    {
	      arc(fout,i*CELLSIZE,j*CELLSIZE,
		  i*CELLSIZE+CELLSIZE/2,j*CELLSIZE+CELLSIZE/8,
		  (i+1)*CELLSIZE,j*CELLSIZE);
	      line(fout,i*CELLSIZE,j*CELLSIZE,(i+1)*CELLSIZE,j*CELLSIZE);
	    }
	  // horizontal arch on bottom
	  if(states[j][i].down == 'v')
	    {
	      arc(fout,(i+1)*CELLSIZE,(j+1)*CELLSIZE,
		  i*CELLSIZE+CELLSIZE/2,(j+1)*CELLSIZE-CELLSIZE/8,
		  i*CELLSIZE,(j+1)*CELLSIZE);
	      line(fout,i*CELLSIZE,(j+1)*CELLSIZE,(i+1)*CELLSIZE,(j+1)*CELLSIZE);
	    }
	  // vertical arch on left
	  if(states[j][i].left == '<')
	    {
	      arc(fout,i*CELLSIZE,(j+1)*CELLSIZE,
		  i*CELLSIZE+CELLSIZE/8,j*CELLSIZE+CELLSIZE/2,
		  i*CELLSIZE,j*CELLSIZE);
	      line(fout,i*CELLSIZE,j*CELLSIZE,i*CELLSIZE,(j+1)*CELLSIZE);
	    }
	  // vertical arch on right
	  if(states[j][i].right == '>')
	    {
	      arc(fout,(i+1)*CELLSIZE,j*CELLSIZE,
		  (i+1)*CELLSIZE-CELLSIZE/8,j*CELLSIZE+CELLSIZE/2,
		  (i+1)*CELLSIZE,(j+1)*CELLSIZE);
	      line(fout,(i+1)*CELLSIZE,j*CELLSIZE,(i+1)*CELLSIZE,(j+1)*CELLSIZE);
	    }
	}
      std::cout << std::endl;
    }
  
  fout.close();
  // Convert Fig file to PDF
  string convert_command = "fig2dev -L pdf "+figFileName+' '+outFileName;
  if(system(convert_command.c_str()) == -1)
    {
      cerr << "printSimPDF() failed to run '"<<convert_command << "' : " <<
	strerror(errno) << endl;
      return false;
    }
  return true;
}

