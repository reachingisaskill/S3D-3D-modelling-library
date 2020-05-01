#define LOGTASTIC_DEBUG_OFF

#include "S3D_tracer_base.h"

#include "S3D_defs.h"
#include "S3D_random.h"

#include "logtastic.h"


namespace S3D
{

  tracer_base::tracer_base() :
    _layers(),
    _lightSamplesPerArea( 0.0 ),
    _totalLightArea( 0.0 ),
    _numLightSamples( 0 )
  {
    _layers.insert( 0 ); // Default layer
  }


  void tracer_base::setup()
  {
    _objectList = ObjectListT();
    _lightList = LightListT();

    ObjectMapT& objectMap = manager::getInstance()->_objects;
    for ( int layer : _layers )
    {
      ObjectMapT::const_iterator layer_it = objectMap.find( layer );
      if ( layer_it == objectMap.end() )
      {
        WARN_STREAM << "tracer_base::setup() : Specified layer is empty: " << layer;
        continue;
      }
      else
      {
        for ( object_base* obj : objectMap[layer] )
        {
          _objectList.insert( obj );
        }
      }
    }

    ObjectContainerT::const_iterator obj_end = _objectList.end();
    for ( ObjectContainerT::const_iterator it = _objectList.begin(); it != obj_end; ++it )
    {
      if ( (*it)->getMaterial()->isLightSource() )
        _lightList.insert( (*it) );
    }

    INFO_STREAM << "Found " << _objectList.size() << " world objects. " << _lightList.size() << " emit light.";

    _totalLightArea = 0.0;
    for ( const object_base* light_obj : _getLights() )
    {
      _totalLightArea += light_obj->getSurfaceArea();
    }


    // Any additional setup
    this->_additionalSetup();


    _numLightSamples = 1 + _lightSamplesPerArea * _totalLightArea;
    INFO_STREAM << " Total emissive area = " << _totalLightArea << ". Sampling rate = " << _lightSamplesPerArea << ". Light samples per vertex: " << _numLightSamples;
  }


  spectrum tracer_base::sampleLightScatter( const object_base* light_obj, const interaction& inter )
  {
    double area = inter.getObject()->getSurfaceArea();
    spectrum the_beam( 0.0, 0.0, 0.0 );
    unsigned int n_samples;
    if ( area < epsilon ) // Just in case its a point source
    {
      n_samples = 1;
    }
    else
    {
      n_samples = 1 + ( _lightSamplesPerArea * area );
    }

    double scaleFactor = 1.0 / (double)n_samples;

    for ( unsigned int n = 0; n < n_samples; ++n )
    {
      surfacemap light_point = light_obj->sampleSurface();

      if ( isVisible( light_point.getPosition(), inter.getPoint(), inter.getSurfaceNormal() ) )
      {
        threeVector lightDir = (inter.getPoint() - light_point.getPosition()).norm();
        interaction light_inter = light_obj->intersect( line( inter.getPoint(), -lightDir ) );
        spectrum light_emission = light_obj->getMaterial()->getExitantRadiance( light_inter );

        DEBUG_STREAM << "tracer_base::sampleLight() : " << light_emission.red() << ", " << light_emission.green() << ", " << light_emission.blue();
        // BRDF * Li
        the_beam += inter.getObject()->getMaterial()->scatter( lightDir, inter ) * light_emission;
      }
      else 
      {
        continue;
      }
    }

    return the_beam;
  }


  spectrum tracer_base::sampleAllLights( const interaction& inter )
  {
    spectrum the_beam( 0.0, 0.0, 0.0 );
    double scaleFactor = 1.0 / (double)_numLightSamples;
    DEBUG_STREAM << "Sampling all lights. " << _numLightSamples << " samples. Scale factor = " << scaleFactor;

    try
    {
      for ( unsigned int n = 0; n < _numLightSamples; ++n )
      {
        const object_base* light_obj = this->_chooseLight();
        DEBUG_STREAM << "  Current Light: " << light_obj;

        surfacemap light_point = light_obj->sampleSurface();

        if ( isVisible( light_point.getPosition(), inter.getPoint(), inter.getSurfaceNormal() ) )
        {
          threeVector lightDir = (inter.getPoint() - light_point.getPosition());
          interaction light_inter = light_obj->intersect( line( inter.getPoint(), -lightDir ) );
          spectrum light_emission = light_obj->getMaterial()->getExitantRadiance( light_inter );


          // samplespace volume * BRDF * Li * Cos_theta
          the_beam += inter.getObject()->getMaterial()->BRDF( lightDir.norm(), inter ) * light_emission * ( -inter.getSurfaceNormal() * lightDir );
        }
        DEBUG_STREAM << "  Current spectrum = " << the_beam.red() << ", " << the_beam.green() << ", " << the_beam.blue();
      }
    }
    catch( stdexts::exception& e )
    {
      std::stringstream ss;
      ss << "Sampling all lights for intersection: " << inter.getPoint().getPosition() << ". Normal = " << inter.getSurfaceNormal();
      EX_LOG( e, ss.str() );
      RETHROW( e );
    }

    return scaleFactor * the_beam;
  }


  spectrum tracer_base::directLighting( const interaction& inter )
  {
    spectrum the_beam( 0.0, 0.0, 0.0 );
    double scaleFactor = 1.0 / (double)_numLightSamples;
    DEBUG_STREAM << "Sampling all lights. " << _numLightSamples << " samples. Scale factor = " << scaleFactor;

    try
    {
      for ( unsigned int n = 0; n < _numLightSamples; ++n )
      {
        const object_base* light_obj = this->_chooseLight();
        DEBUG_STREAM << "  Current Light: " << light_obj;

        surfacemap light_point = light_obj->sampleSurface();

        if ( isVisible( light_point.getPosition(), inter.getPoint(), inter.getSurfaceNormal() ) )
        {
          threeVector lightDir = (inter.getPoint() - light_point.getPosition());
          interaction light_inter = light_obj->intersect( line( inter.getPoint(), -lightDir ) );
          spectrum light_emission = light_obj->getMaterial()->getExitantRadiance( light_inter );


          // samplespace volume * BRDF * Li * Cos_theta
          the_beam += inter.getObject()->getMaterial()->BRDF( lightDir.norm(), inter ) * light_emission * ( -inter.getSurfaceNormal() * lightDir );
        }
        DEBUG_STREAM << "  Current spectrum = " << the_beam.red() << ", " << the_beam.green() << ", " << the_beam.blue();
      }
    }
    catch( stdexts::exception& e )
    {
      std::stringstream ss;
      ss << "Sampling all lights for intersection: " << inter.getPoint().getPosition() << ". Normal = " << inter.getSurfaceNormal();
      EX_LOG( e, ss.str() );
      RETHROW( e );
    }

    return scaleFactor * the_beam;
  }


  const object_base* tracer_base::_chooseLight() const
  {
    double r = random::uniformDouble() * _totalLightArea;
    double currentSum = 0.0;
    const object_base* light_obj;
    LightListT lights = _getLights();
    LightListT::const_iterator lights_end = lights.end();

    for ( LightListT::const_iterator it = lights.begin(); it != lights_end; ++it )
    {
      light_obj = (*it);
      currentSum += light_obj->getSurfaceArea();
      if ( r < currentSum )
        return light_obj;
    }

    FAILURE_LOG( "tracer_base::_chooseLight(): Random number outside range - should not have reached this point... What to do?" );
    stdexts::exception ex( "tracer_base::_chooseLight()", "Random number outside range - should not have reached this point... What to do?" );
    EX_CREATE( ex );
    THROW( ex );
  }


  spectrum tracer_base::traceLightSample( spectrum b, point p, const interaction& inter )
  {
    DEBUG_LOG( "Tracing light sample" );
    if ( isVisible( p, inter.getPoint(), inter.getSurfaceNormal() ) )
    {
      threeVector lightDir = (inter.getPoint() - p).norm();
      // BRDF * Li * cos_theta
      return inter.getObject()->getMaterial()->BRDF( lightDir, inter ) * b * ( -lightDir * inter.getSurfaceNormal() ) ;
    }
    else 
    {
      return spectrum( 0.0, 0.0, 0.0 );
    }
  }


  bool tracer_base::isVisible( point start, point end, threeVector normal ) const
  {
    threeVector separation = end - start;

    DEBUG_STREAM << " tracer_base::isVisible() : " << start.getPosition() << " , " << end.getPosition() << ", " << separation << ". N = " << normal << ". Dot Product = " << separation.norm() * normal;
    if ( ( separation.norm() * normal ) > 0.0 )
    {
      DEBUG_LOG( "  Travelling backwards" );
      return false; // Travelling backwards through surface.
    }

    line the_beam( start, separation );
    double distanceSq = separation.square();

    const ObjectListT& obj_list = _getObjects();
    ObjectListT::const_iterator obj_it = obj_list.begin();
    ObjectListT::const_iterator obj_end = obj_list.end();

    while( obj_it != obj_end )
    {
      if ( (*obj_it)->crosses( the_beam ) )
      {
        interaction intersect = (*obj_it)->intersect( the_beam );

        if ( ( distanceSq - intersect.getDistanceSquared() ) > epsilon )
        {
          DEBUG_STREAM << "  Intersecting Object: " << (*obj_it);
          return false;
        }
      }
      ++obj_it;
    }
    return true;
  }


  interaction tracer_base::getInteraction( line& l )
  {
    const ObjectListT& objects = _getObjects();
    ObjectListT::const_iterator obj_it = objects.begin();
    ObjectListT::const_iterator obj_end = objects.end();

    interaction current_intersect;

    try
    {
      DEBUG_STREAM << "Tracing ray from: " << l.getStart().getPosition() << " with direction: " << l.getDirection();
      while( obj_it != obj_end )
      {
        if ( (*obj_it)->crosses( l ) )
        {
          interaction intersect = (*obj_it)->intersect( l );
          double distanceSq = intersect.getDistanceSquared();
          DEBUG_STREAM << "Object = " << (*obj_it) << ". Distance = " << std::sqrt( distanceSq );

          if ( ( distanceSq > epsilon ) && ( distanceSq < current_intersect.getDistanceSquared() ) )
          {
            current_intersect = intersect;
          }
        }

        ++obj_it;
      }
    }
    catch( stdexts::exception& e )
    {
      std::stringstream ss;
      ss << "Tracing ray from: " << l.getStart().getPosition() << " with direction: " << l.getDirection();
      EX_LOG( e, ss.str() );
      RETHROW( e );
    }

    return current_intersect;
  }


  void tracer_base::addLayer( int l )
  {
    _layers.insert( l );
  }


  void tracer_base::removeLayer( int l )
  {
    auto found =_layers.find( l );
    if ( found == _layers.end() )
    {
      WARN_STREAM << "tracer_base::removeLayer(): Could not find layer '" << l << "' in layers list. Nothing to do.";
    }
    else
    {
      _layers.erase( found );
    }
  }
}

