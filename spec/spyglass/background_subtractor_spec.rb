require 'spec_helper'

describe Spyglass::BackgroundSubtractor do
  let(:bg) { Spyglass::BackgroundSubtractor.new }
  let(:lena) { Spyglass::Image.load(fixture_path('lena.jpg')) }

  describe '.new' do
    it 'should work with no arguments' do
      expect( bg ).to be_a Spyglass::BackgroundSubtractor
    end

    it 'should work with an options hash' do
      expect { Spyglass::BackgroundSubtractor.new(history: 50, threshold: 64) }.not_to raise_error
    end
  end

  describe '#subtract' do
    it 'should return a Spyglass::Image' do
      delta = bg.subtract(lena, 1)
      expect( delta ).to be_a Spyglass::Image
    end
  end
end
