
#ifndef __S3D__SURFACEMAPPING_H__
#define __S3D__SURFACEMAPPING_H__


namespace S3D
{
  // Stores the information required by the material class to perform a surface colour/texture lookup.
  // The most trivial implementation does nothing, forcing the surface colour to be returned.
  class surfaceMapping
  {
    private:
    protected :
    public :
      surfaceMapping() {}
      virtual ~surfaceMapping() {}
  };


  // For when I invent textures....

  class surfaceMapping_2D : public surfaceMapping
  {
  };


  class surfaceMapping_spherical : public surfaceMapping
  {
  };

}

#endif // __S3D__SURFACEMAPPING_H__

