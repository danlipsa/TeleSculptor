/*ckwg +5
 * Copyright 2014 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

/**
 * \file
 * \brief Helper utility functions for getting, setting and checking properties
 *        between OpenCV algorithm properties and maptk::config_block objects.
 */

#ifndef MAPTK_OCV_OCV_ALGO_TOOLS_H
#define MAPTK_OCV_OCV_ALGO_TOOLS_H

#include <string>
#include <iostream>

#include <maptk/core/config_block.h>
#include <maptk/core/exceptions/config_block.h>
#include <maptk/core/types.h>

#include <maptk/ocv/ocv_config.h>

#include <opencv2/core/core.hpp>

namespace maptk
{

namespace ocv
{


/// String used to record nested algorithm implementation type
static std::string const type_token = "type";


namespace helper_
{

/// Helper method for setting nested OpenCV Algorithm parameters
MAPTK_OCV_EXPORT
void set_nested_ocv_algo_configuration_helper(std::string const& name,
                                              config_block_sptr config,
                                              cv::Ptr<cv::Algorithm> &algo);

/// Helper method for checking nested OpenCV Algorithm
/// configurations
MAPTK_OCV_EXPORT
bool check_nested_ocv_algo_configuration_helper(std::string const& name,
                                                config_block_sptr config,
                                                cv::Ptr<cv::Algorithm> algo);

/// Templated helper method and specialization for creating a new OpenCV
/// Algorithm instance.
/**
 * \tparam algo_t cv::Algorithm class or sub-class to attempt creation from. We
 *                will always fall-back to attempting
 *                \c cv::Algorithm::create(impl_name) if creation with the
 *                specific sub-class fails.
 */
template <typename algo_t>
cv::Ptr<algo_t> create_ocv_algo(std::string const& impl_name)
{
  // attempt to use the given type to natively create the algorithm with
  // possible special rules contained in the subclass. If this does not
  // yeild a valid instance, attempt creating an algorithm using the base
  // cv::Algorithm creation rules.
  cv::Ptr<algo_t> a;
  try
  {
    a = algo_t::create(impl_name);
  }
  catch (cv::Exception const&)
  {
    std::cerr << "[---] Ignore the above error message, it will be handled. "
              << "OpenCV is silly."
              << std::endl;
  }

  // if the create call returned something empty or errored, a will still empty.
  if (a.empty())
  {
    a = cv::Algorithm::create<algo_t>(impl_name);
  }
  return a;
}


/// cv::Algorithm specialization
template <>
MAPTK_OCV_EXPORT
cv::Ptr<cv::Algorithm> create_ocv_algo<cv::Algorithm>(std::string const& impl_name);


} // end namespace helper_


/// Add nested OpenCV algorithm's configuration options to the given \c config
/**
 * This includes an algorithm "type" parameter that defines what specific
 * algorithm we are nesting. If the given \c algo is not defined, we only set
 * a blank "type" option. If the given algorithm is defined, we set the "type"
 * parameter to the type of the algorithm given, and fill in that algorithm's
 * options under a subblock of its name.
 *
 * The \c config_block that we are expecting is at the block level for the
 * algorithm that this nested algorithm belongs to.
 *
 * \param name    A \c std::string name for this nested algorithm.
 * \param config  The \c maptk::config_block to add the given algorithm's
 *                options to.
 * \param algo   The cv pointer to the nested algorithm.
 */
MAPTK_OCV_EXPORT
void get_nested_ocv_algo_configuration(std::string const& name,
                                       config_block_sptr config,
                                       cv::Ptr<cv::Algorithm> algo);


/// Set nested OpenCV algorithm's parameters based on a given \c config
/**
 * \tparam algo_t Type of OpenCV algorithm we are dealing with.
 * \param name    A \c std::string name for this nested algorithm. This should
 *                match the name used when \c get_nested_ocv_algo_configuration
 *                was called for this nested algorithm.
 * \param config  The \c maptk::config_block to set OpenCV property values
 *                from.
 * \param algo    The cv pointer to the algorithm to set configuration
 *                options to.
 */
template <typename algo_t>
void set_nested_ocv_algo_configuration(std::string const& name,
                                       config_block_sptr config,
                                       cv::Ptr<algo_t> &algo)
{
  // check that the config has a type for the nested algo, creating a new
  // instance if the given algo is NULL or not of the same type specified
  // in the config.
  config_block_key_t type_key = name + config_block::block_sep + type_token;
  std::string impl_name = config->get_value<std::string>(type_key, "");
  if (impl_name.length() > 0)
  {
    // if the current algo ptr is empty (NULL) or has a type differing from the
    // configured type, create a new algo instance.
    if (algo.empty() || algo->info()->name() != impl_name)
    {
      algo = helper_::create_ocv_algo<algo_t>(impl_name);
    }

    cv::Ptr<cv::Algorithm> converted_algo(algo);
    helper_::set_nested_ocv_algo_configuration_helper(name, config,
                                                      converted_algo);
    algo = cv::Ptr<algo_t>(converted_algo);
  }
  else
  {
    std::cerr << "[WARNING] No algorithm type set for '" << name << "'. "
              << "Using default cv::Algorithm '" << algo->info()->name() << "'."
              << std::endl;
  }
}


/// Basic check of nested OpenCV algorithm configuration in the given \c config
/**
 * If no algorithm type is provided in the configuration, i.e. type parameter
 * not present or blank, we assume the use of defaults, thus returning true.
 *
 * Of all nested algorithm configuration properties that can be encoded within
 * a \c config, check that they are present and that the value in the \c config
 * is castable to the nested algorithm parameter's expected type.
 *
 * We assume that what is pointed to be \c algo is an initialized OpenCV
 * algorithm class.
 *
 * \tparam algo_t Type of OpenCV algorithm we are dealing with.
 * \param name    A \c std::string name for this nested algorithm. This should
 *                match the name used when \c get_nested_ocv_algo_configuration
 *                was called for this nested algorithm.
 * \param config  The \c maptk::config_block to check.
 */
template <typename algo_t>
bool check_nested_ocv_algo_configuration(std::string const& name,
                                         config_block_sptr config)
{
  // use default algo type and parameters if there is no type defined in config
  // or if its value is blank
  config_block_key_t type_key = name + config_block::block_sep + type_token;
  std::string impl_name = config->get_value<std::string>(type_key, "");
  if (impl_name.length() == 0)
  {
    // no specific algorithm type configured, default will be used
    return true;
  }

  // Must have a non-blank type specified in the configuration by this point.
  // Attempt to create an algorithm with the given impl_name and algo_t. If
  // this fails, attempt to apply the given name to the base cv::Algorithm
  // class's creation method. If they both fail, the name is invalid.
  cv::Ptr<algo_t> algo = helper_::create_ocv_algo<algo_t>(impl_name);

  // If the algo creation step returned NULL with the given type name, we
  // assume that the name you provided was invalid for the provided algorithm
  // class.
  if (algo.empty())
  {
    return false;
  }

  return helper_::check_nested_ocv_algo_configuration_helper(name, config, algo);
}


} // end namespace ocv

} // end namespace maptk

#endif // MAPTK_OCV_OCV_ALGO_TOOLS_H
