/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "layeredEngineMesh.H"
#include "addToRunTimeSelectionTable.H"
#include "fvcMeshPhi.H"
#include "surfaceInterpolate.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(layeredEngineMesh, 0);
    addToRunTimeSelectionTable(engineMesh, layeredEngineMesh, IOobject);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::layeredEngineMesh::layeredEngineMesh(const IOobject& io)
:
    engineMesh(io),
    pistonLayers_("pistonLayers", dimLength, Zero)
{
    engineDB_.engineDict().readIfPresent("pistonLayers", pistonLayers_);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::layeredEngineMesh::~layeredEngineMesh()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::layeredEngineMesh::move()
{
    scalar deltaZ = engineDB_.pistonDisplacement().value();
    Info<< "deltaZ = " << deltaZ << endl;

    // Position of the top of the static mesh layers above the piston
    scalar pistonPlusLayers = pistonPosition_.value() + pistonLayers_.value();

    pointField newPoints(points());

    forAll(newPoints, pointi)
    {
        point& p = newPoints[pointi];

        if (p.z() < pistonPlusLayers)           // In piston bowl
        {
            p.z() += deltaZ;
        }
        else if (p.z() < deckHeight_.value())   // In liner region
        {
            p.z() +=
                deltaZ
               *(deckHeight_.value() - p.z())
               /(deckHeight_.value() - pistonPlusLayers);
        }
    }


    auto* phiPtr = engineDB_.getObjectPtr<surfaceScalarField>("phi");

    if (phiPtr)
    {
        auto& phi = *phiPtr;

        const auto& rho = engineDB_.lookupObject<volScalarField>("rho");
        const auto& U = engineDB_.lookupObject<volVectorField>("U");

        const bool absolutePhi = moving();
        if (absolutePhi)
        {
            // cf. fvc::makeAbsolute
            phi += fvc::interpolate(rho)*fvc::meshPhi(rho, U);
        }

        movePoints(newPoints);

        if (absolutePhi)
        {
            // cf. fvc::makeRelative
            phi -= fvc::interpolate(rho)*fvc::meshPhi(rho, U);
        }
    }
    else
    {
        movePoints(newPoints);
    }

    pistonPosition_.value() += deltaZ;
    scalar pistonSpeed = deltaZ/engineDB_.deltaTValue();

    Info<< "clearance: " << deckHeight_.value() - pistonPosition_.value() << nl
        << "Piston speed = " << pistonSpeed << " m/s" << endl;
}


// ************************************************************************* //
