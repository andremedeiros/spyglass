require 'spec_helper'

describe OpenCV::Size do
  let(:square) { OpenCV::Size.new(256, 512) }

  describe '.new' do
    it 'should require two arguments' do
      expect { OpenCV::Size.new }.to raise_error ArgumentError
      expect( square ).to be_a OpenCV::Size
    end
  end

  describe 'accessors' do
    describe '#width' do
      it 'should return the correct width' do
        expect( square.width ).to eq(256)
      end
    end

    describe '#height' do
      it 'should return the correct height' do
        expect( square.height ).to eq(512)
      end
    end

    describe '#area' do
      it 'should return `width * height`' do
        expect( square.area ).to eq(131_072)
      end
    end
  end

  describe 'setters' do
    describe '#width=' do
      it 'should set the width correctly' do
        square.width = 512

        expect( square.width ).to eq(512)
        expect( square.area ).to eq(262_144)
      end
    end

    describe '#height=' do
      it 'should set the height correctly' do
        square.height = 1024

        expect( square.height ).to eq(1024)
        expect( square.area ).to eq(262_144)
      end
    end
  end
end
