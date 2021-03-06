# Roadmap

## 07.2018

* image filtering `filter`:
	- [ ] min/max filter approximation;


## 08.04.2018

* `common/matrix`:      
    - [x] split into 2 templates: fixed size and dynamic size;  
    - [x] initialization with { *, *, ... } fixed size array;
    - [x] use `eigen` as matrix core;
    - [ ] add math operators;

* read/write lossless image formats `imp/io`:
	- [x] PPM 8/16 bit;
	- [ ] BMP;
	- [ ] PNG 8/16 bit;
    
* add lookup table module `lut`:  
	- [ ] 3DLUT;
	- [ ] tetrahedral and trilinear interpolation policy;
	- [ ] 1DLUT;
	- [ ] compile-time LUT initialization;
	- [ ] pre-defined LUTs in float:
		- [ ] sRGB EOTF/OETF;
		- [ ] sRGB => CAM02;
	- [ ] compile-time LUT cast like `lut_cast<uint32_t>(float)`;
	
* colorspace transforms `color`:
	- [ ] EOTF/OETF functions: bt.709, bt.601, sRGB, ST 2084;
	- [ ] RGB <=> XYZ;
	- [ ] XYZ <=> LAB;
	- [ ] XYZ <=> CAM02;
	- [ ] RGB <=> HSL/HSV;
	- [ ] RGB <=> Yuv;	
	- [x] 3 channel image generalization;

* image filtering `filter`:
	- [ ] box filter;
	- [ ] gaussian filter;
	- [ ] median filter;
	- [x] min/max filter;
	- [ ] bilateral filter;
	- [ ] guided filter;
	- [ ] integral image + box filter compatibility;
	
* raw image processing `imp/raw`:
	- [ ] class for CFA;
	- [ ] fuji X-Trans pattern;
	- [ ] demosaic methods:
		- [ ] bilinear;
		- [ ] two-step demosaic: HA, bicubic;
		- [ ] state-of-art;