BerconMaps (Kentebird Fork!)
==========

BerconMaps is a 3rd party texture plugin for Autodesk 3ds Max. It adds five new prodecural texture maps: Noise, Wood, Tile, Distortion and Gradient.

For more information, pictures and binaries go to http://www.ylilammi.com/2013/09/berconmaps/

RELEASE NOTES
=============
Version 3.04.1kf: The AVX binaries are experimental. They may perform more slowly if your system is configured to clock down the CPU when AVX instructions are in use. See https://www.intel.com/content/dam/www/public/us/en/documents/white-papers/performance-xeon-e5-v3-advanced-vector-extensions-paper.pdf for information.

FORK INFORMATION
================
This fork removes the custom control curve, and adds stability back to the program. 
Starting with 3dsmax 2017, Autodesk implemented changes to its program that broke BerconMaps to a certain extent. The largest of these problems was a crash-on-save due to the implementation of the custom control curve. 

I spent a few weeks of full-time work attempting to decipher the cause of the crash but inevitably came to the conclusion that, under the current SDK, it is not possible to implement the custom control curve in its current form. 

It is my position, also, that the custom control curve does not provide any real additional functionality beyond what is included in the 'OUTPUT' map rollup. 

CAUTION is warranted when using this forked version -- your previous scenes may not render the same if you used the custom control curve in your scenes. You may find your scenes render darker or lighter depending on how you had the scene set-up. 

Additional work is currently being performed to optimize the code further to make for faster render times. 

License
-------------------------

Apache License, Version 2.0

Read LICENSE file

Contributing
-------------------------

- Fork
- Improve
- Submit a pull request

Credits
-------------------------

- Jerry Ylilammi (author)
- Vladimir Koylazov (bug fixes and support for 3ds Max 2015)
- Ken Perlin (Perlin/Simplex noise)
- Stefan Gustavson (C++ implementation of Perlin and Simplex noises)
- Steven Worley (Worley noise and C implementation of it)
- Blender team (Fractals)
- John Burnett (Distortions max shadecontext implementation)
- David Baker (Compiled Max 8 version of the plugin and edited source codes for Max 8)
- Vladislav Gavrilov (Helped with integrating my custom gradient control into Max)

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
