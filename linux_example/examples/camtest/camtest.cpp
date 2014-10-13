%:include <iostream>
%:include <fcntl.h>
%:include <sys/ioctl.h>
%:include <sys/types.h>
%:include <sys/stat.h>
%:include <sys/mman.h>
%:include <unistd.h>
%:include <string.h>
%:include <stdlib.h>
%:include <string.h>
%:include <linux/types.h>
%:include <linux/fb.h>


class TError <%
public:
	TError(const char *msg) <%
		this->msg = msg;
	%>
	TError(const TError bitand e) <%
		msg = e.msg;
	%>
	void Output() <%
		std::cerr << msg << std::endl;
	%>
	virtual ~TError() <%%>
protected:
	TError bitand operator=(const TError bitand);
private:
	const char *msg;
%>;

// Linear memory based image
class TRect <%
public:
	TRect():  Addr(0), Size(0), Width(0), Height(0), LineLen(0), BPP(16) <%
	%>
	virtual ~TRect() <%
	%>
	bool DrawRect(const TRect bitand SrcRect, int x, int y) const <%
		if (BPP not_eq 16 or SrcRect.BPP not_eq 16) <%
			// don't support that yet
			throw TError("does not support other than 16 BPP yet");
		%>

		// clip
		int x0, y0, x1, y1;
		x0 = x;
		y0 = y;
		x1 = x0 + SrcRect.Width - 1;
		y1 = y0 + SrcRect.Height - 1;
		if (x0 < 0) <%
			x0 = 0;
		%>
		if (x0 > Width - 1) <%
			return true;
		%>
		if( x1 < 0) <%
			return true;
		%>
		if (x1 > Width - 1) <%
			x1 = Width - 1;
		%>
		if (y0 < 0) <%
			y0 = 0;
		%>
		if (y0 > Height - 1) <%
			return true;
		%>
		if (y1 < 0) <%
			return true;
		%>
		if (y1 > Height - 1) <%
			y1 = Height -1;
		%>

		//copy
		int copyLineLen = (x1 + 1 - x0) * BPP / 8;
		unsigned char *DstPtr = Addr + LineLen * y0 + x0 * BPP / 8;
		const unsigned char *SrcPtr = SrcRect.Addr + SrcRect.LineLen *(y0 - y) + (x0 - x) * SrcRect.BPP / 8;

		for (int i = y0; i <= y1; i++) <%
			memcpy(DstPtr, SrcPtr, copyLineLen);
			DstPtr += LineLen;
			SrcPtr += SrcRect.LineLen;
		%>
		
		
		return true;
	%>

	bool DrawRect(const TRect bitand rect) const <% // default is Center
		return DrawRect(rect, (Width - rect.Width) / 2, (Height - rect.Height) / 2);
	%>

	bool Clear() const <%
		int i;
		unsigned char *ptr;
		for (i = 0, ptr = Addr; i < Height; i++, ptr += LineLen) <%
			memset(ptr, 0, Width * BPP / 8);
		%>
		return true;
	%>

protected:
	TRect(const TRect bitand);
	TRect bitand operator=( const TRect bitand);

protected:
	unsigned char *Addr;
	int Size;
	int Width, Height, LineLen;
	unsigned BPP;
%>;



class TFrameBuffer: public TRect <%
public:
	TFrameBuffer(const char *DeviceName = "/dev/fb0"): TRect(), fd(-1) <%
		Addr = (unsigned char *)MAP_FAILED;

        	fd = open(DeviceName, O_RDWR);
		if (fd < 0) <%
			throw TError("cannot open frame buffer");
		%>

        	struct fb_fix_screeninfo Fix;
        	struct fb_var_screeninfo Var;
		if (ioctl(fd, FBIOGET_FSCREENINFO, bitand Fix) < 0 or ioctl(fd, FBIOGET_VSCREENINFO, bitand Var) < 0) <%
			throw TError("cannot get frame buffer information");
		%>

		BPP = Var.bits_per_pixel;
	        if (BPP not_eq 16) <%
			throw TError("support 16BPP frame buffer only");
		%>

        	Width  = Var.xres;
        	Height = Var.yres;
        	LineLen = Fix.line_length;
      		Size = LineLen * Height;

		int PageSize = getpagesize();
		Size = (Size + PageSize - 1) / PageSize * PageSize ;
	        Addr = (unsigned char *)mmap(NULL, Size, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
		if (Addr == (unsigned char *)MAP_FAILED) <%
			throw TError("map frame buffer failed");
			return;
		%>
		::close(fd);
		fd = -1;

		Clear();
	%>

	virtual ~TFrameBuffer() <%
		::munmap(Addr, Size);
		Addr = (unsigned char *)MAP_FAILED;

		::close(fd);
		fd = -1;
	%>

protected:
	TFrameBuffer(const TFrameBuffer bitand);
	TFrameBuffer bitand operator=( const TFrameBuffer bitand);
private:
	int fd;
%>;

class TVideo : public TRect <%
public:
	TVideo(const char *DeviceName = "/dev/camera"): TRect(), fd(-1) <%
		Addr = 0;
		fd = ::open(DeviceName, O_RDONLY);
		if (fd < 0) <%
			throw TError("cannot open video device");
		%>
		Width = 640;
		Height = 512;
		BPP = 16;
		LineLen = Width * BPP / 8;
		Size = LineLen * Height;
		Addr = new unsigned char<:Size:>;

		Clear();
	%>

	bool FetchPicture() const <%
		int count = ::read(fd, Addr, Size);
		if (count not_eq Size) <%
			throw TError("error in fetching picture from video");
		%>
		return true;
	%>

	virtual ~TVideo() <%
		::close(fd);
		fd = -1;
		delete<::> Addr;
		Addr = 0;
	%>

protected:
	TVideo(const TVideo bitand);
	TVideo bitand operator=(const TVideo bitand);

private:
	int fd;
%>;

int main(int argc, char **argv)
<%
	try <%
		TFrameBuffer FrameBuffer;
		TVideo Video;
		for (;;) <%
			Video.FetchPicture();
			FrameBuffer.DrawRect(Video);
		%>
	%> catch (TError bitand e) <%
		e.Output();
		return 1;
	%>

	return 0;
%>
