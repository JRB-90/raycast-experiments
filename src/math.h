#ifndef _MATH_H_
#define _MATH_H_

// Data types
struct Vertex {
	double x;
	double y;
};

struct Frame {
	double x;
	double y;
	double theta;
};

struct Line {
	struct Vertex p1;
	struct Vertex p2;
};

// Typedefs
typedef struct Vertex Vertex;
typedef struct Frame Frame;
typedef struct Line Line;

// Function defs
extern Vertex CreateDefaultVertex();
extern Vertex CreateVertex(double x, double y);
extern Frame CreateDefaultFrame();
extern Frame CreateFrame(double x, double y, double theta);
extern Line CreateLine(Vertex p1, Vertex p2);

extern Vertex CalculateOffsetPoint(Frame frame, double offset);

#endif // !_MATH_H_
