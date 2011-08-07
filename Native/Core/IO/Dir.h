/*
 *
 *  Created by Peter Zion on 10-12-07.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */
 
#ifndef _FABRIC_IO_DIR_H
#define _FABRIC_IO_DIR_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/ConstHandle.h>

#include <vector>
#include <string>

namespace Fabric
{
  namespace IO
  {
    class Dir : public RC::Object
    {
    public:
    
      static RC::ConstHandle<Dir> Root();
      static RC::ConstHandle<Dir> User();
      static RC::ConstHandle<Dir> Private();
          
      static RC::ConstHandle<Dir> Create( RC::ConstHandle<Dir> const &parentDir, std::string path, bool createIfMissing = true )
      {
        return new Dir( parentDir, path, createIfMissing );
      }
      
      RC::ConstHandle<Dir> getParentDir() const
      {
        return m_parentDir;
      }
      
      std::string const &getEntry() const
      {
        return m_entry;
      }

      bool exists() const;
      
      std::string getFullPath() const;
      
      std::vector< std::string > getFiles() const;
      std::vector< RC::ConstHandle<Dir> > getSubDirs() const;
      
      std::string getFileContents( std::string const &entry ) const;
      void putFileContents( std::string const &entry, std::string const &contents ) const;
    
    protected:
    
      Dir( RC::ConstHandle<Dir> const &parentDir, std::string const &entry, bool createIfMissing );
      
    private:
    
      RC::ConstHandle<Dir> m_parentDir;
      std::string m_entry;
    };
  };
};

#endif //_FABRIC_IO_DIR_H
