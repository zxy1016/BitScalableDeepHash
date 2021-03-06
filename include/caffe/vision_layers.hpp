// Copyright 2013 Yangqing Jia

#ifndef CAFFE_VISION_LAYERS_HPP_
#define CAFFE_VISION_LAYERS_HPP_

#include <leveldb/db.h>
#include <pthread.h>
#include <opencv2/core/core.hpp>

#include <vector>
#include <string>
#include <map>

#include "caffe/layer.hpp"
#include "caffe/proto/caffe.pb.h"
namespace caffe {

// The neuron layer is a specific type of layers that just works on single
// celements.
template<typename Dtype>
class NeuronLayer: public Layer<Dtype> {
public:
	explicit NeuronLayer(const LayerParameter& param) :
			Layer<Dtype>(param) {
	}
	virtual void SetUp(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
};

template<typename Dtype>
class ReLULayer: public NeuronLayer<Dtype> {
public:
	explicit ReLULayer(const LayerParameter& param) :
			NeuronLayer<Dtype>(param) {
	}

protected:
	virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

	virtual Dtype Backward_cpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	virtual Dtype Backward_gpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
};

template<typename Dtype>
class SigmoidLayer: public NeuronLayer<Dtype> {
public:
	explicit SigmoidLayer(const LayerParameter& param) :
			NeuronLayer<Dtype>(param) {
	}
	virtual void SetUp(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

protected:
	virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

	virtual Dtype Backward_cpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	virtual Dtype Backward_gpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	Dtype sigm_decay;
	Dtype sigm_para;
	int iter_decay;
};

template<typename Dtype>
class BNLLLayer: public NeuronLayer<Dtype> {
public:
	explicit BNLLLayer(const LayerParameter& param) :
			NeuronLayer<Dtype>(param) {
	}

protected:
	virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

	virtual Dtype Backward_cpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	virtual Dtype Backward_gpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
};

template<typename Dtype>
class DropoutLayer: public NeuronLayer<Dtype> {
public:
	explicit DropoutLayer(const LayerParameter& param) :
			NeuronLayer<Dtype>(param) {
	}
	virtual void SetUp(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

protected:
	virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

	virtual Dtype Backward_cpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	virtual Dtype Backward_gpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	shared_ptr<SyncedMemory> rand_vec_;
	float threshold_;
	float scale_;
	unsigned int uint_thres_;
};


template<typename Dtype>
class ElementWiseProductLayer: public Layer<Dtype> {
public:
	explicit ElementWiseProductLayer(const LayerParameter& param) :
			Layer<Dtype>(param) {
	}
	virtual void SetUp(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

protected:
	virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

	virtual Dtype Backward_cpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	virtual Dtype Backward_gpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	int M_;
	int K_;
	int N_;
	bool biasterm_;
	shared_ptr<SyncedMemory> bias_multiplier_;
};







template<typename Dtype>
class FlattenLayer: public Layer<Dtype> {
public:
	explicit FlattenLayer(const LayerParameter& param) :
			Layer<Dtype>(param) {
	}
	virtual void SetUp(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

protected:
	virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual Dtype Backward_cpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	virtual Dtype Backward_gpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	int count_;
};

template<typename Dtype>
class InnerProductLayer: public Layer<Dtype> {
public:
	explicit InnerProductLayer(const LayerParameter& param) :
			Layer<Dtype>(param) {
	}
	virtual void SetUp(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

protected:
	virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

	virtual Dtype Backward_cpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	virtual Dtype Backward_gpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	int M_;
	int K_;
	int N_;
	bool biasterm_;
	shared_ptr<SyncedMemory> bias_multiplier_;
};

template<typename Dtype>
class PaddingLayer: public Layer<Dtype> {
public:
	explicit PaddingLayer(const LayerParameter& param) :
			Layer<Dtype>(param) {
	}
	virtual void SetUp(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

protected:
	virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual Dtype Backward_cpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	virtual Dtype Backward_gpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	unsigned int PAD_;
	int NUM_;
	int CHANNEL_;
	int HEIGHT_IN_;
	int WIDTH_IN_;
	int HEIGHT_OUT_;
	int WIDTH_OUT_;
};

template<typename Dtype>
class LRNLayer: public Layer<Dtype> {
public:
	explicit LRNLayer(const LayerParameter& param) :
			Layer<Dtype>(param) {
	}
	virtual void SetUp(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

protected:
	virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual Dtype Backward_cpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	virtual Dtype Backward_gpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	// scale_ stores the intermediate summing results
	Blob<Dtype> scale_;
	int size_;
	int pre_pad_;
	Dtype alpha_;
	Dtype beta_;
	int num_;
	int channels_;
	int height_;
	int width_;
};

template<typename Dtype>
class Im2colLayer: public Layer<Dtype> {
public:
	explicit Im2colLayer(const LayerParameter& param) :
			Layer<Dtype>(param) {
	}
	virtual void SetUp(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

protected:
	virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual Dtype Backward_cpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	virtual Dtype Backward_gpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	int KSIZE_;
	int STRIDE_;
	int CHANNELS_;
	int HEIGHT_;
	int WIDTH_;
};

template<typename Dtype>
class PoolingLayer: public Layer<Dtype> {
public:
	explicit PoolingLayer(const LayerParameter& param) :
			Layer<Dtype>(param) {
	}
	virtual void SetUp(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

protected:
	virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual Dtype Backward_cpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	virtual Dtype Backward_gpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	int KSIZE_;
	int STRIDE_;
	int CHANNELS_;
	int HEIGHT_;
	int WIDTH_;
	int POOLED_HEIGHT_;
	int POOLED_WIDTH_;
	Blob<float> rand_idx_;
};

template<typename Dtype>
class ConvolutionLayer: public Layer<Dtype> {
public:
	explicit ConvolutionLayer(const LayerParameter& param) :
			Layer<Dtype>(param) {
	}
	virtual void SetUp(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

protected:
	virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual Dtype Backward_cpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	virtual Dtype Backward_gpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	Blob<Dtype> col_bob_;

	int KSIZE_;
	int STRIDE_;
	int NUM_;
	int CHANNELS_;
	int HEIGHT_;
	int WIDTH_;
	int NUM_OUTPUT_;
	int GROUP_;
	Blob<Dtype> col_buffer_;
	shared_ptr<SyncedMemory> bias_multiplier_;
	bool biasterm_;
	int M_;
	int K_;
	int N_;
};

// This function is used to create a pthread that prefetches the data.
template<typename Dtype>
void* DataLayerPrefetch(void* layer_pointer);

template<typename Dtype>
class DataLayer: public Layer<Dtype> {
	// The function used to perform prefetching.
	friend void* DataLayerPrefetch<Dtype>(void* layer_pointer);

public:
	explicit DataLayer(const LayerParameter& param) :
			Layer<Dtype>(param) {
	}
	virtual ~DataLayer();
	virtual void SetUp(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

	virtual int getDataCount() const {
		return counts_;
	}

	virtual const vector<std::string>& getFilenames() const {
		return filenames_;
	}

//	virtual shared_ptr<vector<std::string> >& getMutableFilenames() {
//		return filenames_;
//	}

//	virtual const vector<cv::Point>& getOffset() const {
//		return offset_;
//	}

//	virtual shared_ptr<vector<cv::Point> >& getMutableOffset() {
//		return offset_;
//	}

//	virtual const Blob<Dtype>* getLabels() const {
//		return &(*labels_);
//	}

//	virtual void setCrop_bboxes(const Blob<Dtype>& tmp_bboxes) {
//		if (Caffe::mode() == Caffe::CPU) {
//			memcpy(crop_bboxes_->mutable_cpu_data(), tmp_bboxes.cpu_data(),
//					sizeof(Dtype) * crop_bboxes_->count());
//		} else {
//			CUDA_CHECK(
//					cudaMemcpy(crop_bboxes_->mutable_gpu_data(),
//							tmp_bboxes.gpu_data(),
//							sizeof(Dtype) * crop_bboxes_->count(),
//							cudaMemcpyHostToDevice));
//		}
//	}

//	virtual const std::map<std::string, vector<int> >& getDeformation() {
//		return deformation_;
//	}
//
//	virtual void setDeformation(std::string filename, vector<int> deformation) {
//		deformation_[filename] = deformation;
//	}

protected:
	virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual Dtype Backward_cpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	virtual Dtype Backward_gpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);

//	shared_ptr<leveldb::DB> db_;
//	shared_ptr<leveldb::Iterator> iter_;
	int datum_channels_;
	int datum_height_;
	int datum_width_;
	int datum_size_;

	pthread_t thread_;
	shared_ptr<Blob<Dtype> > prefetch_data_;
	shared_ptr<Blob<Dtype> > prefetch_W_;
//	shared_ptr<Blob<Dtype> > prefetch_label_;
	Blob<Dtype> data_mean_;

	// zhu
	int counts_;
	int batch_iter_;
	int img_counts_per_class_per_iter_;
//	shared_ptr<vector<std::string> > filenames_;
//	shared_ptr<vector<cv::Point> > offset_;
//	vector<std::string> filenames_;
//	vector<cv::Point> offset_;
//	shared_ptr<Blob<Dtype> > labels_;
//	shared_ptr<vector<std::string> > prefetch_filenames_;
//	shared_ptr<vector<cv::Point> > prefetch_offset_;
	// segmentation根据这个crop_bboxs_对图片进行裁剪，然后只对裁剪后的图片做分割，
	// 去掉的部分都为背景，但是算loss的时候只对裁剪后的图片算
//	shared_ptr<Blob<Dtype> > crop_bboxes_;
	// 根据公式推出的最优deformation
//	std::map<std::string, vector<int> > deformation_;

	// 每个类对应的id
	std::map<std::string, int> class2id_;
	// 所有类名
	std::vector<std::string> class_names_;
	// 每个类包含的图片数目
	std::vector<int> img_counts_per_class_;
	// 所有图片的名字
	std::vector<std::string> filenames_;
	// 每个类选取的图片时候最小的id
	int beg_index_;
	// 下一张要读取的图片的id;
	int curIndex;
};

template<typename Dtype>
class SoftmaxLayer: public Layer<Dtype> {
public:
	explicit SoftmaxLayer(const LayerParameter& param) :
			Layer<Dtype>(param) {
	}
	virtual void SetUp(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

protected:
	virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual Dtype Backward_cpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	virtual Dtype Backward_gpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);

	// sum_multiplier is just used to carry out sum using blas
	Blob<Dtype> sum_multiplier_;
	// scale is an intermediate blob to hold temporary results.
	Blob<Dtype> scale_;
};

template<typename Dtype>
class MultinomialLogisticLossLayer: public Layer<Dtype> {
public:
	explicit MultinomialLogisticLossLayer(const LayerParameter& param) :
			Layer<Dtype>(param) {
	}
	virtual void SetUp(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

protected:
	// The loss layer will do nothing during forward - all computation are
	// carried out in the backward pass.
	virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top) {
		return;
	}
	virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top) {
		return;
	}
	virtual Dtype Backward_cpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	// virtual Dtype Backward_gpu(const vector<Blob<Dtype>*>& top,
	//     const bool propagate_down, vector<Blob<Dtype>*>* bottom);
};

template<typename Dtype>
class InfogainLossLayer: public Layer<Dtype> {
public:
	explicit InfogainLossLayer(const LayerParameter& param) :
			Layer<Dtype>(param), infogain_() {
	}
	virtual void SetUp(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

protected:
	// The loss layer will do nothing during forward - all computation are
	// carried out in the backward pass.
	virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top) {
		return;
	}
	virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top) {
		return;
	}
	virtual Dtype Backward_cpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	// virtual Dtype Backward_gpu(const vector<Blob<Dtype>*>& top,
	//     const bool propagate_down, vector<Blob<Dtype>*>* bottom);

	Blob<Dtype> infogain_;
};

// SoftmaxWithLossLayer is a layer that implements softmax and then computes
// the loss - it is preferred over softmax + multinomiallogisticloss in the
// sense that during training, this will produce more numerically stable
// gradients. During testing this layer could be replaced by a softmax layer
// to generate probability outputs.
template<typename Dtype>
class SoftmaxWithLossLayer: public Layer<Dtype> {
public:
	explicit SoftmaxWithLossLayer(const LayerParameter& param) :
			Layer<Dtype>(param), softmax_layer_(new SoftmaxLayer<Dtype>(param)) {
	}
	virtual void SetUp(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

protected:
	virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual Dtype Backward_cpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	virtual Dtype Backward_gpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);

	shared_ptr<SoftmaxLayer<Dtype> > softmax_layer_;
	// prob stores the output probability of the layer.
	Blob<Dtype> prob_;
	// Vector holders to call the underlying softmax layer forward and backward.
	vector<Blob<Dtype>*> softmax_bottom_vec_;
	vector<Blob<Dtype>*> softmax_top_vec_;
};

template<typename Dtype>
class SigmoidWithLossLayer: public Layer<Dtype> {
public:
	explicit SigmoidWithLossLayer(const LayerParameter& param) :
			Layer<Dtype>(param), sigmoid_layer_(new SigmoidLayer<Dtype>(param)) {
	}
	virtual void SetUp(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

protected:
	virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	virtual Dtype Backward_cpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	virtual Dtype Backward_gpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);

	shared_ptr<SigmoidLayer<Dtype> > sigmoid_layer_;
	// prob stores the output probability of the layer.
	Blob<Dtype> prob_;
	// Vector holders to call the underlying softmax layer forward and backward.
	vector<Blob<Dtype>*> sigmoid_bottom_vec_;
	vector<Blob<Dtype>*> sigmoid_top_vec_;
};

template<typename Dtype>
class EuclideanLossLayer: public Layer<Dtype> {
public:
	explicit EuclideanLossLayer(const LayerParameter& param) :
			Layer<Dtype>(param), difference_() {
	}
	virtual void SetUp(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

protected:
	// The loss layer will do nothing during forward - all computation are
	// carried out in the backward pass.
	virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top) {
		return;
	}
	virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top) {
		return;
	}
	virtual Dtype Backward_cpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	// virtual Dtype Backward_gpu(const vector<Blob<Dtype>*>& top,
	//     const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	Blob<Dtype> difference_;
};

template<typename Dtype>
class EuclideanTripletLossLayer: public Layer<Dtype> {
public:
	explicit EuclideanTripletLossLayer(const LayerParameter& param) :
			Layer<Dtype>(param), difference_() {
	}
	virtual void SetUp(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

protected:
	// The loss layer will do nothing during forward - all computation are
	// carried out in the backward pass.
	virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top) {
		return;
	}
	virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top) {
		return;
	}
	virtual Dtype Backward_cpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	// virtual Dtype Backward_gpu(const vector<Blob<Dtype>*>& top,
	//     const bool propagate_down, vector<Blob<Dtype>*>* bottom);
	Blob<Dtype> difference_;

	Dtype loss_threshold_;
	Dtype laplacian_beta_;
	Blob<Dtype> s_;
	int iter_decay;
	Dtype decay;

};

template<typename Dtype>
class AccuracyLayer: public Layer<Dtype> {
public:
	explicit AccuracyLayer(const LayerParameter& param) :
			Layer<Dtype>(param) {
	}
	virtual void SetUp(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);

protected:
	virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
			vector<Blob<Dtype>*>* top);
	// The accuracy layer should not be used to compute backward operations.
	virtual Dtype Backward_cpu(const vector<Blob<Dtype>*>& top,
			const bool propagate_down, vector<Blob<Dtype>*>* bottom) {
		NOT_IMPLEMENTED;
		return Dtype(0.);
	}
};

}  // namespace caffe

#endif  // CAFFE_VISION_LAYERS_HPP_

