#include "Cubie.h"

using namespace irr;

Cubie::Cubie(scene::ISceneManager* manager, scene::ISceneNode* parent, const core::vector3df& position, const std::array<video::SColor, 6>& faces)
: cubieFaces{ CubieFaceMesh{ faces[0] }, CubieFaceMesh{ faces[1] }, CubieFaceMesh{ faces[2] }, CubieFaceMesh{ faces[3] }, CubieFaceMesh{ faces[4] }, CubieFaceMesh{ faces[5] }}
{
  positionNode = manager->addEmptySceneNode(parent);
  positionNode->setPosition(position);

  rotationNode = manager->addEmptySceneNode(positionNode);

  static const std::array<core::vector3df, 6> positions
  {
    core::vector3df{ 0, -0.5f, 0 },
    core::vector3df{ 0, +0.5f, 0 },
    core::vector3df{ 0, 0, +0.5f },
    core::vector3df{ -0.5f, 0, 0 },
    core::vector3df{ 0, 0, -0.5f },
    core::vector3df{ +0.5f, 0, 0 }
  };
  static const std::array<core::vector3df, 6> rotations
  {
    core::vector3df{ +90, 0, 0 },
    core::vector3df{ -90, 0, 0 },
    core::vector3df{ 0, 0, 0 },
    core::vector3df{ 0, -90, 0 },
    core::vector3df{ 0, +180, 0 },
    core::vector3df{ 0, +90, 0 }
  };
  for (size_t i = 0; i < 6; ++i)
  {
    faceSceneNodes[i] = manager->addMeshSceneNode(&cubieFaces[i].GetMesh(), rotationNode, -1, positions[i], rotations[i]);
    faceSceneNodes[i]->setMaterialFlag(video::EMF_LIGHTING, false);
  }
}

void Cubie::ApplyMove(Face face, f32 degreesClockwise, f32 minDistanceFromCentre)
{
  auto position = positionNode->getPosition();
  const f32 epsilon = 0.0001f;

  switch (face)
  {
    case Face::Up:
      if (minDistanceFromCentre - positionNode->getPosition().Y < epsilon)
      {
        position.rotateXZBy(-degreesClockwise);
        ApplyRotation(core::vector3df{ 0, +degreesClockwise, 0 });
      }
      break;

    case Face::Down:
      if (minDistanceFromCentre + positionNode->getPosition().Y < epsilon)
      {
        position.rotateXZBy(+degreesClockwise);
        ApplyRotation(core::vector3df{ 0, -degreesClockwise, 0 });
      }
      break;

    case Face::Front:
      if (minDistanceFromCentre - positionNode->getPosition().Z < epsilon)
      {
        position.rotateXYBy(+degreesClockwise);
        ApplyRotation(core::vector3df{ 0, 0, +degreesClockwise });
      }
      break;

    case Face::Right:
      if (minDistanceFromCentre + positionNode->getPosition().X < epsilon)
      {
        position.rotateYZBy(-degreesClockwise);
        ApplyRotation(core::vector3df{ -degreesClockwise, 0, 0 });
      }
      break;

    case Face::Back:
      if (minDistanceFromCentre + positionNode->getPosition().Z < epsilon)
      {
        position.rotateXYBy(-degreesClockwise);
        ApplyRotation(core::vector3df{ 0, 0, -degreesClockwise });
      }
      break;

    case Face::Left:
      if (minDistanceFromCentre - positionNode->getPosition().X < epsilon)
      {
        position.rotateYZBy(+degreesClockwise);
        ApplyRotation(core::vector3df{ +degreesClockwise, 0, 0 });
      }
      break;
  }

  positionNode->setPosition(position);
}

void Cubie::ApplyRotation(const irr::core::vector3df& rotation)
{
  core::matrix4 baseRotationMatrix;
  baseRotationMatrix.setRotationDegrees(rotationNode->getRotation());

  core::matrix4 appliedRotationMatrix;
  appliedRotationMatrix.setRotationDegrees(rotation);

  auto combinedRotationMatrix = appliedRotationMatrix * baseRotationMatrix;
  rotationNode->setRotation(combinedRotationMatrix.getRotationDegrees());
}
