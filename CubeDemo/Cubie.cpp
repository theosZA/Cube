#include "Cubie.h"

using namespace irr;

Cubie::Cubie(scene::ISceneManager* manager, irr::scene::ISceneNode* parent, const irr::core::vector3df& position, const std::array<video::SColor, 6>& faces)
: cubieFaces{ CubieFaceMesh{ faces[0] }, CubieFaceMesh{ faces[1] }, CubieFaceMesh{ faces[2] }, CubieFaceMesh{ faces[3] }, CubieFaceMesh{ faces[4] }, CubieFaceMesh{ faces[5] }}
{
  positionNode = manager->addEmptySceneNode(parent);
  positionNode->setPosition(position);

  rotationNode = manager->addEmptySceneNode(positionNode);

  static const std::array<core::vector3df, 6> positions
  {
    core::vector3df{ 0, -5, 0 },
    core::vector3df{ 0, +5, 0 },
    core::vector3df{ 0, 0, +5 },
    core::vector3df{ -5, 0, 0 },
    core::vector3df{ 0, 0, -5 },
    core::vector3df{ +5, 0, 0 }
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

void Cubie::ApplyMove(CubeMove move)
{
  auto position = positionNode->getPosition();

  switch (move.face)
  {
    case Face::Up:
      if (positionNode->getPosition().Y > 0)
      {
        position.rotateXZBy(-90 * move.quarterRotationsClockwise);
        ApplyRotation(core::vector3df{ 0, +90.0f * move.quarterRotationsClockwise, 0 });
      }
      break;

    case Face::Down:
      if (positionNode->getPosition().Y < 0)
      {
        position.rotateXZBy(+90 * move.quarterRotationsClockwise);
        ApplyRotation(core::vector3df{ 0, -90.0f * move.quarterRotationsClockwise, 0 });
      }
      break;

    case Face::Front:
      if (positionNode->getPosition().Z > 0)
      {
        position.rotateXYBy(90 * move.quarterRotationsClockwise);
        ApplyRotation(core::vector3df{ 0, 0, +90.0f * move.quarterRotationsClockwise });
      }
      break;

    case Face::Right:
      if (positionNode->getPosition().X < 0)
      {
        position.rotateYZBy(-90 * move.quarterRotationsClockwise);
        ApplyRotation(core::vector3df{ -90.0f * move.quarterRotationsClockwise, 0, 0 });
      }
      break;

    case Face::Back:
      if (positionNode->getPosition().Z < 0)
      {
        position.rotateXYBy(-90 * move.quarterRotationsClockwise);
        ApplyRotation(core::vector3df{ 0, 0, -90.0f * move.quarterRotationsClockwise });
      }
      break;

    case Face::Left:
      if (positionNode->getPosition().X > 0)
      {
        position.rotateYZBy(+90 * move.quarterRotationsClockwise);
        ApplyRotation(core::vector3df{ +90.0f * move.quarterRotationsClockwise, 0, 0 });
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
