/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2017 Wikki Ltd
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::fa::fourthLnGrad

Description
    Fourth-order snGrad scheme with non-orthogonal correction.

SourceFiles
    fourthLnGrad.C

\*---------------------------------------------------------------------------*/

#ifndef fourthLnGrad_H
#define fourthLnGrad_H

#include "lnGradScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fa
{

/*---------------------------------------------------------------------------*\
                        Class fourthLnGrad Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class fourthLnGrad
:
    public lnGradScheme<Type>
{
public:

    //- Runtime type information
    TypeName("fourth");


    // Generated Methods

        //- No copy construct
        fourthLnGrad(const fourthLnGrad&) = delete;

        //- No copy assignment
        void operator=(const fourthLnGrad&) = delete;


    // Constructors

        //- Construct from mesh
        fourthLnGrad(const faMesh& mesh)
        :
            lnGradScheme<Type>(mesh)
        {}


        //- Construct from mesh and data stream
        fourthLnGrad(const faMesh& mesh, Istream&)
        :
            lnGradScheme<Type>(mesh)
        {}


    //- Destructor
    virtual ~fourthLnGrad();


    // Member Functions

        //- Return the interpolation weighting factors for the given field
        virtual tmp<edgeScalarField> deltaCoeffs
        (
            const GeometricField<Type, faPatchField, areaMesh>&
        ) const
        {
            return this->mesh().deltaCoeffs();
        }

        //- Return true if this scheme uses an explicit correction
        virtual bool corrected() const { return true; }

        //- Return the explicit correction to the fourthLnGrad
        //  for the given field
        virtual tmp<GeometricField<Type, faePatchField, edgeMesh>>
        correction(const GeometricField<Type, faPatchField, areaMesh>&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fa

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "fourthLnGrad.C"
#else
    #ifdef xlC
    #pragma implementation("fourthLnGrad.C")
    #endif
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
