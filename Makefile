CXXFLAGS = -g3 -Wall -fmessage-length=0 -Wno-class-memaccess -Wno-deprecated -Wno-char-subscripts -fPIE
LDFLAGS = -fPIE

OBJS = wordshooter.o util.o

LIBS = -L/usr/X11R6/lib -lm -lpthread -lglut -lGLU -lGL -lX11 -lfreeimage

TARGET = word-shooter

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
