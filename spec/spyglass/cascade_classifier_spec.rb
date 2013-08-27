require 'spec_helper'

describe Spyglass::CascadeClassifier do
  let(:classifier) { Spyglass::CascadeClassifier.new(fixture_path('haarcascade_frontalface_default.xml')) }
  let(:lena) { Spyglass::Image.load(fixture_path('lena.jpg')) }

  describe '.new' do
    it 'should receive an argument' do
      expect { Spyglass::CascadeClassifier.new }.to raise_error ArgumentError;
      expect( classifier ).to be_a Spyglass::CascadeClassifier
    end
  end

  describe '.detect' do
    it 'should return an array of Spyglass::Rect' do
      rects = classifier.detect(lena)

      expect( rects ).to be_a Array

      rect = rects.first
      expect( rect ).to         be_a Spyglass::Rect
      expect( rect.x ).to       be_close_to 215, threshold: 10
      expect( rect.y ).to       be_close_to 202, threshold: 10
      expect( rect.width ).to   be_close_to 174, threshold: 10
      expect( rect.height ).to  be_close_to 174, threshold: 10
    end
  end
end
