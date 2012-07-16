/*
 * Software License Agreement (BSD License)
 *
 *  Point Cloud Library (PCL) - www.pointclouds.org
 *  Copyright (c) 2012, Willow Garage, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <pcl/apps/modeler/abstract_worker.h>
#include <pcl/apps/modeler/parameter_dialog.h>
#include <pcl/apps/modeler/cloud_item.h>

#include <QCoreApplication>

//////////////////////////////////////////////////////////////////////////////////////////////
pcl::modeler::AbstractWorker::AbstractWorker(const std::vector<CloudItem*>& polymeshs, QWidget* parent) :
  clouds_(polymeshs),
  parameter_dialog_(new ParameterDialog(getName(), parent))
{
}

//////////////////////////////////////////////////////////////////////////////////////////////
pcl::modeler::AbstractWorker::~AbstractWorker(void)
{
  delete parameter_dialog_;
}

//////////////////////////////////////////////////////////////////////////////////////////////
int
pcl::modeler::AbstractWorker::exec()
{
  for (size_t i = 0, i_end = clouds_.size(); i < i_end; ++ i)
    initParameters(clouds_[i]->getCloud());

  setupParameters();

  return (parameter_dialog_->exec());
}


//////////////////////////////////////////////////////////////////////////////////////////////
void
pcl::modeler::AbstractWorker::process()
{
  for (size_t i = 0, i_end = clouds_.size(); i < i_end; ++ i)
    processImpl(clouds_[i]);

  emit processed();

  return;
}


//////////////////////////////////////////////////////////////////////////////////////////////
void
pcl::modeler::AbstractWorker::postProcess()
{
  moveToThread(QCoreApplication::instance()->thread());

  for (size_t i = 0, i_end = clouds_.size(); i < i_end; ++ i)
    postProcessImpl(clouds_[i]);

  emit finished();

  return;
}
