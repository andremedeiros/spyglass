require 'spec_helper'

describe Spyglass::Color do
  let(:color) { Spyglass::Color.new(255, 123, 234) }

  describe '.new' do
    it 'should accept between 1 to 4 arguments' do
      expect( Spyglass::Color.new(255) ).to be_a Spyglass::Color
      expect( Spyglass::Color.new(255, 255) ).to be_a Spyglass::Color
      expect( Spyglass::Color.new(255, 255, 255) ).to be_a Spyglass::Color
      expect( Spyglass::Color.new(255, 255, 255, 255) ).to be_a Spyglass::Color

      expect { Spyglass::Color.new }.to raise_error ArgumentError
    end
  end

  describe 'getters' do
    describe '#to_a' do
      it 'should return an array with the color values' do
        expected  = [255, 123, 234, 0]
        colors    = color.to_a

        expect( colors ).to eq(expected)
      end
    end

    describe '#[]' do
      it 'should return the correct color values for indices between 0..3' do
        expected  = [255, 123, 234, 0]
        colors    = color.to_a

        4.times do |idx|
          expect( color[idx] ).to eq(expected[idx])
        end
      end

      it 'should return nil for anything < 0 or > 3' do
        expect( color[-2] ).to be_nil
        expect( color[4] ).to be_nil
      end
    end

    describe '#zeros?' do
      it 'should return true when the color is comprised only by zeros' do
        zeros = Spyglass::Color.new(0, 0, 0, 0)
        expect( zeros.zeros? ).to be_true
      end

      it 'should return false when the color has at least one element that is not zero' do
        expect( color.zeros? ).to be_false
      end
    end
  end
end
