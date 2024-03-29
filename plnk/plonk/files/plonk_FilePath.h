/*
 -------------------------------------------------------------------------------
 This file is part of the Plink, Plonk, Plank libraries
  by Martin Robinson
 
 http://code.google.com/p/pl-nk/
 
 Copyright University of the West of England, Bristol 2011-14
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
 * Neither the name of University of the West of England, Bristol nor 
   the names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 DISCLAIMED. IN NO EVENT SHALL UNIVERSITY OF THE WEST OF ENGLAND, BRISTOL BE 
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
 GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
 OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 
 This software makes use of third party libraries. For more information see:
 doc/license.txt included in the distribution.
 -------------------------------------------------------------------------------
 */

#ifndef PLONK_FILEPATH_H
#define PLONK_FILEPATH_H

#include "plonk_FilesForwardDeclarations.h"

class FilePathInternal : public SmartPointer
{
public:
    PLONK_INLINE_LOW FilePathInternal() throw()
    {
        pl_Path_Init (&peer);
    }
    
    PLONK_INLINE_LOW FilePathInternal (const char* path) throw()
    {
        if ((path[0] == '~') && (strlen (path) > 1))
            pl_Path_InitSystem (&peer, PLANKPATH_SYSTEMUSERHOME, (path[1] == '/') ? path + 2 : path + 1);
        else
            pl_Path_InitPath (&peer, path);
    }

    PLONK_INLINE_LOW ~FilePathInternal()
    {
        pl_Path_DeInit (&peer);
    }
    
    friend class FilePath;
    
private:
    PlankPath peer;
};

/** File path construction, manipulation and operations.
 A semi-cross platform file path class. Rules are:
 - internally, the path separator must be '/' the native() method can be used to return
   a '\\' sepatated path on Windows if desired.
 - paths to a directory ALWAYS will and must end in a trailing '/'
 - paths created from explicit text containg parent shortcuts i.e., '../' will be
   kept in the internal string until the resolve() function is called, the child(),
   parent() and sibling() functions default to resolving these paths when constructing
   new paths.
 */
class FilePath : public SmartPointerContainer<FilePathInternal>
{
public:
    typedef SmartPointerContainer<FilePathInternal> Base;
    
    enum System
    {
        UserHome        = PLANKPATH_SYSTEMUSERHOME,
        UserDocuments   = PLANKPATH_SYSTEMUSERDOCUMENTS,
        UserDesktop     = PLANKPATH_SYSTEMUSERDESKTOP,
        UserAppData     = PLANKPATH_SYSTEMUSERAPPDATA,
        AppData         = PLANKPATH_SYSTEMAPPDATA,
        App             = PLANKPATH_SYSTEMAPP,
        Temp            = PLANKPATH_SYSTEMTEMP
    };
    
    PLONK_INLINE_LOW FilePath() throw()
    :   Base (new FilePathInternal (""))
    {
    }

    PLONK_INLINE_LOW FilePath (const char* path) throw()
    :   Base (new FilePathInternal (path))
    {
    }
    
    PLONK_INLINE_LOW FilePath (Text const& path) throw()
    :   Base (new FilePathInternal (path.getArray()))
    {
    }
    
    PLONK_INLINE_LOW static FilePath system (const System type) throw()
    {
        FilePath file (new FilePathInternal());
        pl_Path_InitSystem (&file.getInternal()->peer, type, 0);
        return file;
    }
        
    PLONK_INLINE_LOW static FilePath temp() throw()
    {
        FilePath file (new FilePathInternal());
        pl_Path_InitTemp (&file.getInternal()->peer, 0, 0);
        return file;
    }
    
    PLONK_INLINE_LOW static FilePath temp (Text const& prefix) throw()
    {
        FilePath file (new FilePathInternal());
        pl_Path_InitTemp (&file.getInternal()->peer, prefix.getArray(), 0);
        return file;
    }
    
    PLONK_INLINE_LOW static FilePath temp (Text const& prefix, Text const& extension) throw()
    {
        FilePath file (new FilePathInternal());
        pl_Path_InitTemp (&file.getInternal()->peer, prefix.getArray(), extension.getArray());
        return file;
    }
    
    PLONK_INLINE_LOW FilePath parent() const throw()
    {
        return parent (true);
    }
    
    PLONK_INLINE_LOW FilePath child (FilePath const& relative) const throw()
    {
        return child (relative.fullpath(), true);
    }

    PLONK_INLINE_LOW FilePath sibling (FilePath const& relative) const throw()
    {
        return sibling (relative.fullpath(), true);
    }

    PLONK_INLINE_LOW FilePath parent (const bool shouldResolve) const throw()
    {
        FilePath file (new FilePathInternal());
        pl_Path_InitParent (&file.getInternal()->peer, &this->getInternal()->peer);
        return shouldResolve ? file.resolve() : file;
    }
    
    PLONK_INLINE_LOW FilePath child (FilePath const& relative, const bool shouldResolve) const throw()
    {
        FilePath file (new FilePathInternal());
        pl_Path_InitChild (&file.getInternal()->peer, &this->getInternal()->peer, relative.fullpath());
        return shouldResolve ? file.resolve() : file;
    }
    
    PLONK_INLINE_LOW FilePath sibling (FilePath const& relative, const bool shouldResolve) const throw()
    {
        FilePath file (new FilePathInternal());
        pl_Path_InitSibling (&file.getInternal()->peer, &this->getInternal()->peer, relative.fullpath());
        return shouldResolve ? file.resolve() : file;
    }
    
    PLONK_INLINE_LOW Text fullpath() const throw()
    {
        return pl_Path_GetFullPath (&this->getInternal()->peer);
    }
    
    PLONK_INLINE_LOW Text root() const throw()
    {
        return pl_Path_GetRoot (&this->getInternal()->peer);
    }

    PLONK_INLINE_LOW Text filename() const throw()
    {
        return pl_Path_GetFilename (&this->getInternal()->peer);
    }

    PLONK_INLINE_LOW Text extension() const throw()
    {
        return pl_Path_GetFileExtension (&this->getInternal()->peer);
    }

    PLONK_INLINE_LOW Text filenameWithoutExtension() const throw()
    {
        Text filename = pl_Path_GetFilename (&this->getInternal()->peer);
        
        for (int i = filename.length(); --i >= 0;)
        {
            if (filename.atUnchecked (i) == '.')
            {
                filename.put (i, '\0');
                break;
            }
        }
        
        return filename;
    }
    
    PLONK_INLINE_LOW FilePath withExtension (Text const& ext) const throw()
    {
        return (ext.first() == '.') ? FilePath (fullpath() + ext) : FilePath (fullpath() + Text (".") + ext);
    }

    PLONK_INLINE_LOW bool isFile() const throw()
    {
        return pl_Path_IsFile (&this->getInternal()->peer);
    }
    
    PLONK_INLINE_LOW bool isDirectory() const throw()
    {
        return pl_Path_IsDirectory (&this->getInternal()->peer);
    }
    
    PLONK_INLINE_LOW operator Text () const throw()
    {
        return fullpath();
    }
    
    PLONK_INLINE_LOW Text native() const throw()
    {
#if PLONK_WIN
        return fullpath().replaceIgnoreCase ("/", "\\");
#else
        return fullpath();
#endif
    }
    
    PLONK_INLINE_LOW FilePath& resolve() throw()
    {
        pl_Path_Resolve (&this->getInternal()->peer);
        return *this;
    }
    
    PLONK_INLINE_LOW bool exists() const throw()
    {
        return pl_FileExists (fullpath(), isDirectory());
    }

    PLONK_INLINE_LOW bool create() const throw()
    {
        if (isDirectory())
        {
            return pl_FileMakeDirectory (fullpath()) == PlankResult_OK;
        }
        else
        {
            Text parentPath = parent().fullpath();
            
            if (!pl_FileExists (parentPath.getArray(), true))
            {
                bool success = pl_FileMakeDirectory (parentPath);
                
                if (!success)
                    return false;
            }
            
            PlankFile file;
            pl_File_Init (&file);
            PlankResult result = pl_File_OpenBinaryWrite (&file, fullpath(), false, false, false);
            
            if (result != PlankResult_OK)
                return false;
            
            return pl_File_DeInit (&file) == PlankResult_OK;
        }
        
        return false;
    }
    
    PLONK_INLINE_LOW bool createParent() const throw()
    {
        return parent().create();
    }
    
    PLONK_INLINE_LOW bool erase() const throw()
    {
        return pl_FileErase (fullpath()) == PlankResult_OK;
    }
    
private:
    FilePath (FilePathInternal* internal) throw()
    :   Base (internal)
    {
    }

};

#endif // PLONK_FILEPATH_H
