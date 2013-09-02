require 'spec_helper'

describe Spyglass::Contour do
  let(:empty_contour) { Spyglass::Contour.new }
  let(:contour) do
    points = 10.times.map { |i| Spyglass::Point.new(i + 1, i + 1) }
    Spyglass::Contour.new points
  end

  describe '.new' do
    it 'should work without arguments' do
      expect( empty_contour ).to be_a Spyglass::Contour
    end

    it 'should work with an array of points' do
      expect { contour }.not_to raise_error
    end
  end

  describe '#rect' do
    it 'should return the bounding rect' do
      expect( contour.rect.size.width ).to eq(10)
      expect( contour.rect.size.height ).to eq(10)
    end
  end
end
