import numpy as np
import pandas as pd

class BayesianNetwork:
    def __init__(self, n_labels=10, n_pixels=784, n_values=2) -> None:
        '''
        n_labels: number of labels, 10 for digit recognition
        n_pixels: number of pixels, 256 for 16x16 image
        n_values: number of values for each pixel, 0 for black, 1 for white
        '''
        self.n_labels = n_labels
        self.n_pixels = n_pixels
        self.n_values = n_values
        # prior probability
        self.labels_prior = np.zeros(n_labels)
        self.pixels_prior = np.zeros((n_pixels, n_values))
        # conditional probability
        self.pixels_cond_label = np.zeros((n_pixels, n_values, n_labels))

    # fit the model with training data
    def fit(self, pixels, labels):
        '''
        pixels: (n_samples, n_pixels, )
        labels: (n_samples, )
        '''
        n_samples = len(labels)
        # 计算先验概率
        label_counts = np.zeros(self.n_labels)
        unique_labels, counts = np.unique(labels, return_counts=True)
        for unique_label, count in zip(unique_labels, counts):
            label_counts[unique_label] = count
        # 上面的操作是为了避免出现某个数字在训练集中没有出现的情况
        self.labels_prior = label_counts / n_samples
        # 计算条件概率
        for label in range(self.n_labels):
            # 当前数字对应的像素集合
            pixels_for_label = pixels[labels == label]
            pixels_for_label_counts = len(pixels_for_label)
            # 计算每个像素在给定数字时的条件概率
            for pixel_index in range(self.n_pixels):
                pixel_value_counts = np.zeros(self.n_values)
                if pixels_for_label_counts > 0:
                    distinct_pixel_values, counts = np.unique(pixels_for_label[:, pixel_index], return_counts=True)
                    for unique_value, count in zip(distinct_pixel_values, counts):
                        pixel_value_counts[unique_value] = count
                self.pixels_cond_label[pixel_index, :, label] = pixel_value_counts / pixels_for_label_counts

    # predict the labels for new data
    def predict(self, pixels):
        '''
        pixels: (n_samples, n_pixels, )
        return labels: (n_samples, )
        '''
        n_samples = len(pixels)
        labels = np.zeros(n_samples)
        for i in range(n_samples):
            posterior = np.copy(self.labels_prior)
            for pixel_index in range(self.n_pixels):
                pixel_value = pixels[i, pixel_index]
                posterior *= self.pixels_cond_label[pixel_index, pixel_value, :]
            labels[i] = np.argmax(posterior)
        return labels

    # calculate the score (accuracy) of the model
    def score(self, pixels, labels):
        '''
        pixels: (n_samples, n_pixels, )
        labels: (n_samples, )
        '''
        n_samples = len(labels)
        labels_pred = self.predict(pixels)
        return np.sum(labels_pred == labels) / n_samples


if __name__ == '__main__':
    # load data
    train_data = np.loadtxt('../data/train.csv', delimiter=',', dtype=np.uint8)
    test_data = np.loadtxt('../data/test.csv', delimiter=',', dtype=np.uint8)
    pixels_train, labels_train = train_data[:, :-1], train_data[:, -1]
    pixels_test, labels_test = test_data[:, :-1], test_data[:, -1]
    # build bayesian network
    bn = BayesianNetwork()
    bn.fit(pixels_train, labels_train)
    print('test score: %f' % bn.score(pixels_test, labels_test))