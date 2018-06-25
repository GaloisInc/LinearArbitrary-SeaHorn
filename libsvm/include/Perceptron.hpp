/**
 * Single Layer Perceptron (SLP) on C++.
 * Copyright (C) 2015  Jean Pierre Dudey

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef PERCEPTRON_HPP
#define PERCEPTRON_HPP

#include <vector>
#include <numeric>
#include <stdexcept>

#include "TrainingItem.hpp"
#include "polynomial.h"

class Perceptron
{
 public:
  Perceptron (int input_count, const double learning_rate = 0.1,
              const double threshold = 0.0)
    : weights_(input_count)
  {
    learning_rate_ = learning_rate;
    threshold_ = threshold;
  }

  double learning_rate() const { return learning_rate_; }
  void learning_rate(const double &learning_rate) { learning_rate_ = learning_rate; }

  double threshold() const { return threshold_; }
  void threshold(const double &threshold) { threshold_ = threshold; }

  std::vector<double> & weights() { return weights_; }
  void weights(const std::vector<double> &weights) { weights_ = weights; }
  
  void train(std::vector<TrainingItem> & training_set, unsigned int max_iterations)
  {
    if (max_iterations == 0)
      throw std::invalid_argument("The maximum number of iterations cannot be 0.");

    unsigned int iterations(0);
    while (true) {
        if (iterations > max_iterations)
            break;
        else
            iterations++;

        int error_count = 0;

        for (auto &item : training_set) {
            bool output = learn(item.output(), item.inputs());

            if (output != item.output())
                error_count++;
        }

        if (error_count == 0)
            break; 
    }
  }

  bool get_result(const std::vector<double> &inputs)
  {
    if (inputs.size() != weights_.size())
      throw std::invalid_argument("Invalid number of inputs. Expected!");
                                

    return dot_product(inputs, weights_) > threshold_;
  }

  bool learn(bool expected_result, const std::vector<double> &inputs)
  {
    bool result = get_result(inputs);
    if (result != expected_result) {
        // Convert boolean to a number
        double error = (expected_result ? 1 : -1) - (result ? 1 : -1);
        for (int i = 0; i < weights_.size(); i++) {
            weights_[i] += learning_rate_ * error * inputs[i];
        }
    }

    return result;
  }
  
  void display (Polynomial* poly = NULL) {
  	double theta[Cv0to4];// = poly->theta;
	for (int i = 0; i < Cv0to4; i++)
		theta[i] = 0;
	  
  	poly->setDims(DIMENSION + 1);
	
	for (int i = 0; i < weights_.size(); i++) {
		theta[i] = weights_[i];
	}

	poly->set(theta);
  }

 private:
  double learning_rate_;
  double threshold_;
  std::vector<double> weights_;

  /**
   * Taken from StackOverflow:
   * http://stackoverflow.com/questions/13314098/get-the-dot-product-of-two-vectors-by-functors-and-stl-algorithms
   */
  double dot_product(const std::vector<double> &v1,
                     const std::vector<double> &v2)
  {
      return std::inner_product(v1.begin(), v1.end(), v2.begin(), 0.0);
  }

};

#endif // PERCEPTRON_HPP
