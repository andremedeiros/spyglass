require 'spec_helper'

describe OpenCV::CascadeClassifier do
  let(:classifier) { OpenCV::CascadeClassifier.new(fixture_path('haarcascade_frontalface_default.xml')) }
  let(:lena) { OpenCV::Image.load(fixture_path('lena.jpg')) }

  describe '.new' do
    it 'should receive an argument' do
      expect { OpenCV::CascadeClassifier.new }.to raise_error ArgumentError;
      expect( classifier ).to be_a OpenCV::CascadeClassifier
    end
  end

  describe '.detect' do
    it 'should return an array of OpenCV::Rect' do
      rects = classifier.detect(lena)

      expect( rects ).to be_a Array

      rect = rects.first
      expect( rect ).to be_a OpenCV::Rect
      expect( rect.x ).to be_close_to 215, 10
      expect( rect.y ).to be_close_to 202, 10
      expect( rect.width ).to be_close_to 174, 10
      expect( rect.height ).to be_close_to 174, 10
    end
  end
end
